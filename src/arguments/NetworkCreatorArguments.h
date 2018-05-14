#ifndef NETWORKCREATORARGUMENTS_H
#define NETWORKCREATORARGUMENTS_H

#include "Arguments.h"
#include <string>
#include <vector>
#include "../utils/Defaults.h"

namespace nn2048 {

class NetworkCreatorArguments : public Arguments
{
public:
    std::vector<unsigned> networkStructure;
    std::string networkFileName;
    double weightDistribution = DefaultWeightDistribution;
    bool createFannNetwork = false;

    const static std::string NetworkStructureArgument;
    const static std::string NetworkFileNameArgument;
    const static std::string WeightDistributionArgument;
    const static std::string CreateFannNetworkArgument;
};

}

#endif // NETWORKCREATORARGUMENTS_H
