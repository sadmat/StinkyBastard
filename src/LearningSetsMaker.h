#ifndef LEARNINGSETSMAKER_H
#define LEARNINGSETSMAKER_H

#include "Application.h"
#include <string>
#include <array>
#include <map>
#include <GameHistorySerializer.h>
#include <LearningSetSerializer.h>
#include "arguments/LearningSetsMakerArguments.h"

namespace nn2048
{

typedef std::array<unsigned, static_cast<size_t>(Game2048Core::Direction::Total)> MoveStats;
typedef std::vector<std::vector<double>> NormalizedBoardState;
typedef std::map<NormalizedBoardState, MoveStats> MoveStatsMap;

class LearningSetsMaker: public Application
{
public:
    LearningSetsMaker(std::unique_ptr<LearningSetsMakerArguments> arguments);

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
    std::unique_ptr<LearningSetsMakerArguments> _arguments;
};

}

#endif // LEARNINGSETSMAKER_H
