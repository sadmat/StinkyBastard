#ifndef NETWORKCREATORARGUMENTSPARSER_H
#define NETWORKCREATORARGUMENTSPARSER_H

#include "ArgumentParser.h"
#include <vector>

namespace nn2048 {

class NetworkCreatorArgumentsParser : public ArgumentParser
{
public:
    NetworkCreatorArgumentsParser(int argc, char **argv);

    std::unique_ptr<Arguments> parsedArguments();

private:
    bool parseNetworkStructure(std::vector<unsigned> &output);
    bool parseNetworkFileName(std::string &output);
    bool parseWeightDistribution(double &output);
    bool parseCreateFannNetwork(bool &output);
};


}

#endif // NETWORKCREATORARGUMENTSPARSER_H
