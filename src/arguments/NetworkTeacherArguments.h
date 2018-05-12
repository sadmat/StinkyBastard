#ifndef NETWORKTEACHERARGUMENTS_H
#define NETWORKTEACHERARGUMENTS_H

#include "Arguments.h"
#include <string>
#include "../utils/Defaults.h"

namespace nn2048 {

class NetworkTeacherArguments : public Arguments
{
public:
    std::string networkFileName;
    std::string replayMemoryDirectory;
    unsigned maxEpochs = 0;
    double minError = 0.0;
    double learningRate = DefaultLearningRate;
    double momentum = DefaultMomentumFactor;
    double gamma = DefaultGammaFactor;

    const static std::string NetworkFileNameArgument;
    const static std::string ReplayMemoryDirectoryArgument;
    const static std::string MaxEpochsArgument;
    const static std::string MinErrorArgument;
    const static std::string LearningRateArgument;
    const static std::string MomentumFactorArgument;
    const static std::string GammaFactorArgument;
};

}

#endif // NETWORKTEACHERARGUMENTS_H
