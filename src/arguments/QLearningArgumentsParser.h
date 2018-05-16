#ifndef QLEARNINGARGUMENTSPARSER_H
#define QLEARNINGARGUMENTSPARSER_H

#include "ArgumentParser.h"

namespace nn2048 {

class QLearningArgumentsParser : public ArgumentParser
{
public:
    QLearningArgumentsParser(int argc, char **argv);

    std::unique_ptr<Arguments> parsedArguments();

private:
    bool parseNetworkFileName(std::string &output);
    bool parseMaxAge(unsigned &output);
    bool parseTargetScore(unsigned &output);
    bool parseGammaFactor(double &output);
    bool parseLearningRate(double &output);
    bool parseMomentumFactor(double &output);
    bool parseEpsilonFactor(double &output);
    bool parseReplayMemorySize(unsigned &output);
    bool parseReplayBatchSize(unsigned &output);
    bool parseReplayMemoryFileName(std::string &output);
};

}

#endif // QLEARNINGARGUMENTSPARSER_H
