#ifndef NETWORKTEACHERARGUMENTSPARSER_H
#define NETWORKTEACHERARGUMENTSPARSER_H

#include "ArgumentParser.h"

namespace nn2048 {

class NetworkTeacherArgumentsParser : public ArgumentParser
{
public:
    NetworkTeacherArgumentsParser(int argc, char *argv[]);

    std::unique_ptr<Arguments> parsedArguments();

private:
    bool parseNetworkFileName(std::string &output);
    bool parseReplayMemoryDirectory(std::string &output);
    bool parseMaxEpochs(unsigned &output);
    bool parseMinError(double &output);
    bool parseLearningRate(double &output);
    bool parseMomentumFactor(double &output);
};

}

#endif // NETWORKTEACHERARGUMENTSPARSER_H
