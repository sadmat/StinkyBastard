#ifndef WEBAPPARGUMENTS_H
#define WEBAPPARGUMENTS_H

#include "Arguments.h"
#include <string>
#include "../utils/Defaults.h"

namespace nn2048 {

class WebAppArguments : public Arguments
{
public:
    std::string argv0;
    unsigned short port = DefaultServerPort;
    std::string serverName;
    std::string documentRoot;
    std::string resourcesDirectory;
    std::string appRootDirectory;
    std::string neuralNetworkFileName;
    unsigned long highscoreThreshold = DefaultHighscoreToRecordThreshold;

    static const std::string PortArgument;
    static const std::string ServerNameArgument;
    static const std::string DocumentRootArgument;
    static const std::string ResourcesDirectoryArgument;
    static const std::string AppRootDirectoryArgument;
    static const std::string NeuralNetworkFileNameArgument;
    static const std::string HighscoreThresholdArgument;
};

}



#endif // WEBAPPARGUMENTS_H
