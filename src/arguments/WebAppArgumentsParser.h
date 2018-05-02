#ifndef WEBAPPARGUMENTSPARSER_H
#define WEBAPPARGUMENTSPARSER_H

#include "ArgumentParser.h"

namespace nn2048 {

class WebAppArgumentsParser : public ArgumentParser
{
public:
    WebAppArgumentsParser(int argc, char **argv);

    std::unique_ptr<Arguments> parsedArguments();

private:
    bool parseArgv0(std::string &output);
    bool parsePort(unsigned short &output);
    bool parseServerName(std::string &output);
    bool parseDocumentRoot(std::string &output);
    bool parseResourcesDirectory(std::string &output);
    bool parseAppRootDirectory(std::string &output);
    bool parseNeuralNetworkFileName(std::string &output);
    bool parseHighscoreThreshold(unsigned long &output);
};

}


#endif // WEBAPPARGUMENTSPARSER_H
