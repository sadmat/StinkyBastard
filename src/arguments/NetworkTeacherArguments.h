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
    std::string learningSetsFileName;
    unsigned maxEpochs = 0;
    double minError = 0.0;
    double learningRate = DefaultLearningRate;
    double momentum = DefaultMomentumFactor;

    const static std::string NetworkFileNameArgument;
    const static std::string LearningSetsFileNameArgument;
    const static std::string MaxEpochsArgument;
    const static std::string MinErrorArgument;
    const static std::string LearningRateArgument;
    const static std::string MomentumFactorArgument;
};

}

#endif // NETWORKTEACHERARGUMENTS_H
