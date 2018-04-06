#include "BoardSignalConverter.h"
#include <cmath>

namespace nn2048
{

std::vector<double> BoardSignalConverter::boardToSignal(const Game2048Core::BoardState &board)
{
    std::vector<double> signal;
    signal.reserve(board.size() * board.size());
    double log2maxValue = std::log2(maxTileValue(board));

    for (auto &row: board)
    {
        for (auto &tile: row)
        {
            double log2value = 0.0;
            if (tile.value() > 0)
                log2value = std::log2(tile.value());
            signal.push_back(log2value / log2maxValue);
        }
    }

    return signal;
}

double BoardSignalConverter::maxTileValue(const Game2048Core::BoardState &board)
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

std::vector<double> BoardSignalConverter::boardToBitSignal(const Game2048Core::BoardState &board)
{
    size_t numberOfTiles = 16;
    size_t numberOfPossibleValues = 16;
    auto signal = std::vector<double>(numberOfPossibleValues * numberOfTiles);
    size_t index = 0;

    for (auto &row: board) {
        for (auto &tile: row) {
            if (tile.value() > 0) {
                auto offset = static_cast<size_t>(std::log2(tile.value()));
                signal[index + offset - 1] = 1.0;
            }
            index += numberOfPossibleValues;
        }
    }

    return signal;
}

}


