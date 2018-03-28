#ifndef QLEARNINGARGUMENTS_H
#define QLEARNINGARGUMENTS_H

#include <string>
#include "../utils/Defaults.h"

namespace nn2048 {

class QLearningArguments
{
public:
    std::string networkFileName = "";
    unsigned maxAge = 0;
    unsigned targetScore = 0;
    double gamma = DefaultGammaFactor;
    double learningRate = DefaultLearningRate;
    double momentumFactor = DefaultMomentumFactor;
    double epsilonFactor = DefaultEpsilonFactor;
    unsigned replayMemorySize = DefaultReplayMemorySize;
    unsigned replayBatchSize = DefaultReplayBatchSize;

    const static std::string NetworkFileNameArgument;
    const static std::string MaxAgeArgument;
    const static std::string TargetScoreArgument;
    const static std::string GammaFactorArgument;
    const static std::string LearningRateArgument;
    const static std::string MomentumFactorArgument;
    const static std::string EpsilonFactorArgument;
    const static std::string ReplayMemorySizeArgument;
    const static std::string ReplayBatchSize;
};

}

#endif // QLEARNINGARGUMENTS_H
