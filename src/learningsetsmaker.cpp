#include "learningsetsmaker.h"
#include <boost/filesystem.hpp>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <fstream>

namespace Stinky
{

using namespace boost;

LearningSetsMaker::LearningSetsMaker(const std::string &recordsDirectory, const std::string &outputFileName, unsigned minScore):
    _recordsDirectory(recordsDirectory),
    _outputFileName(outputFileName),
    _minScore(minScore)
{}

int LearningSetsMaker::run()
{
    try
    {
        // Get file list
        auto jsons = jsonFileNames();
        if (jsons.size() == 0)
        {
            std::cerr << "No jsons found in " << _recordsDirectory << std::endl;
            return 0;
        }

        // Extract moves
        MoveStatsMap map = moveStatsForJsons(jsons);

        // Normalize outputs
        std::cout << "Producing learning sets... ";
        auto learningSets = createLearningSets(map);
        std::cout << "ok" << std::endl;

        serializeLearningSets(learningSets);
    }
    catch (const std::invalid_argument &exception)
    {
        std::cerr << "Invalid argument: " << exception.what() << std::endl;
    }
    catch (const std::runtime_error &exception)
    {
        std::cerr << "Runtime error: " << exception.what() << std::endl;
    }
    return 0;
}

std::vector<std::string> LearningSetsMaker::jsonFileNames() const
{
    if (!filesystem::is_directory(_recordsDirectory))
    {
        std::string message = _recordsDirectory + " is not a directory";
        throw std::invalid_argument(message);
    }

    std::vector<std::string> jsons;
    for (const filesystem::directory_entry &entry: filesystem::directory_iterator(_recordsDirectory))
    {
        if (entry.path().extension() == ".json")
            jsons.push_back(entry.path().string());
    }
    return jsons;
}

MoveStatsMap LearningSetsMaker::moveStatsForJsons(const std::vector<std::string> &jsons) const
{
    MoveStatsMap map;
    unsigned accepted = 0;
    unsigned duplicates = 0;
    for (const auto &json: jsons)
    {
        try
        {
            unsigned highscore = 0;
            std::cout << "Parsing " << json << "... ";
            auto moves = Game2048Core::GameHistorySerializer::deserialize(json, &highscore);
            std::cout << "ok" << std::endl;
            if (highscore < _minScore)
            {
                std::cout << "Highscore " << highscore << " < " << _minScore << " - rejecting" << std::endl;
                continue;
            }

            std::cout << "Processing moves... ";
            duplicates += processMoves(moves, map);
            std::cout << "ok" << std::endl;
            ++accepted;
        }
        catch (const std::runtime_error &exception)
        {
            std::cout << "failed" << std::endl;
            std::cerr << "Exception caught during deserialization: " << exception.what() << std::endl;
        }
    }
    std::cout << "Accepted files: " << accepted << ", ";
    std::cout << "loaded states: " << map.size() << ", ";
    std::cout << "duplicated: " << duplicates << std::endl;
    return map;
}

unsigned LearningSetsMaker::processMoves(std::list<Game2048Core::Move> &moves, MoveStatsMap &map) const
{
    unsigned duplicates = 0;
    for (auto &move: moves)
    {
        auto normalized = normalizeBoard(move.first.board);
        if (map.count(normalized) == 0)
        {
            if (move.second != Game2048Core::Direction::None)
            {
                MoveStats stats;
                for (auto &stat: stats) stat = 0;
                stats[(size_t)move.second] = 1;
                map[normalized] = stats;
            }
        }
        else
        {
            map[normalized][(size_t)move.second]++;
            ++duplicates;
        }
    }
    return duplicates;
}

NormalizedBoardState LearningSetsMaker::normalizeBoard(const Game2048Core::BoardState &board) const
{
    NormalizedBoardState normalizedBoard;
    normalizedBoard.reserve(board.size());

    double log2maxVal = maxTileValue(board);
    if (log2maxVal > 0.0)
        log2maxVal = std::log2(log2maxVal);

    for (auto &row: board)
    {
        std::vector<double> normalizedRow;
        normalizedRow.reserve(row.size());

        for (auto &tile: row)
        {
            if (tile.value() > 0)
            {
                double log2val = std::log2(tile.value());
                normalizedRow.push_back(log2val / log2maxVal);
            }
            else normalizedRow.push_back(0.0);
        }
        normalizedBoard.push_back(std::move(normalizedRow));
    }
    return normalizedBoard;
}

double LearningSetsMaker::maxTileValue(const Game2048Core::BoardState &board) const
{
    double value = 0;
    for (auto &row: board)
    {
        for (auto &tile: row)
        {
            if (tile.value() > value)
                value = tile.value();
        }
    }
    return value;
}

std::vector<NeuralNetwork::LearningSet> LearningSetsMaker::createLearningSets(const MoveStatsMap &map) const
{
    std::vector<NeuralNetwork::LearningSet> sets;
    sets.reserve(map.size());

    for (auto &pair: map)
    {
        const NormalizedBoardState &board = pair.first;
        const MoveStats &moves = pair.second;
        NeuralNetwork::LearningSet set;
        set.input.reserve(board.size() * board.size());
        set.output.reserve(moves.size());

        for (auto &row: board)
            for (auto &value: row)
                set.input.push_back(value);

        unsigned maxValue = *(std::max_element(moves.begin(), moves.end()));
        for (auto value: moves)
            set.output.push_back(value / maxValue);

        sets.push_back(std::move(set));
    }
    return sets;
}

void LearningSetsMaker::serializeLearningSets(const std::vector<NeuralNetwork::LearningSet> &learningSets) const
{
    try
    {
        std::cout << "Serializing... ";
        std::cout.flush();
        std::ofstream file(_outputFileName);
        if (!file.is_open())
        {
            std::string message = "Could not open file: " + _outputFileName;
            throw std::runtime_error(message);
        }
        NeuralNetwork::LearningSetSerializer::serialize(learningSets, file);
        file.close();
        std::cout << "ok" << std::endl;
    }
    catch (std::exception &ex)
    {
        std::cout << std::endl;
        throw;
    }
}

}


