#ifndef LEARNINGSETSMAKER_H
#define LEARNINGSETSMAKER_H

#include "Application.h"
#include <string>
#include <array>
#include <map>
#include <GameHistorySerializer.h>
#include <LearningSetSerializer.h>

namespace nn2048
{

typedef std::array<unsigned, (size_t)Game2048Core::Direction::Total> MoveStats;
typedef std::vector<std::vector<double>> NormalizedBoardState;
typedef std::map<NormalizedBoardState, MoveStats> MoveStatsMap;

class LearningSetsMaker: public Application
{
public:
    LearningSetsMaker(const std::string &recordsDirectory, const std::string &outputFileName, unsigned minScore = 0);

    int run();

protected:
    std::vector<std::string> jsonFileNames() const;
    MoveStatsMap moveStatsForJsons(const std::vector<std::string> &jsons) const;
    unsigned processMoves(std::list<Game2048Core::Move> &moves, MoveStatsMap &map) const;
    NormalizedBoardState normalizeBoard(const Game2048Core::BoardState &board) const;
    double maxTileValue(const Game2048Core::BoardState &board) const;
    std::vector<NeuralNetwork::LearningSet> createLearningSets(const MoveStatsMap &map) const;
    void serializeLearningSets(const std::vector<NeuralNetwork::LearningSet> &learningSets) const;

private:
    std::string _recordsDirectory;
    std::string _outputFileName;
    unsigned _minScore;
};

}

#endif // LEARNINGSETSMAKER_H
