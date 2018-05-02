#include "WebAppArgumentsParser.h"
#include <iostream>
#include "WebAppArguments.h"

namespace nn2048 {

WebAppArgumentsParser::WebAppArgumentsParser(int argc, char **argv):
    ArgumentParser(argc, argv, 2)
{}

std::unique_ptr<Arguments> WebAppArgumentsParser::parsedArguments()
{
    auto arguments = std::make_unique<WebAppArguments>();
    arguments->argv0 = _argv[0];
    for (; _currentArgIndex < static_cast<unsigned>(_argc); ++_currentArgIndex) {
        auto currentArg = _argv[_currentArgIndex];
        if (currentArg == WebAppArguments::PortArgument) {
            if (!parsePort(arguments->port))
                return nullptr;
        } else if (currentArg == WebAppArguments::ServerNameArgument) {
            if (!parseServerName(arguments->serverName))
                return nullptr;
        } else if (currentArg == WebAppArguments::DocumentRootArgument) {
            if (!parseDocumentRoot(arguments->documentRoot))
                return nullptr;
        } else if (currentArg == WebAppArguments::ResourcesDirectoryArgument) {
            if (!parseResourcesDirectory(arguments->resourcesDirectory))
                return nullptr;
        } else if (currentArg == WebAppArguments::AppRootDirectoryArgument) {
            if (!parseAppRootDirectory(arguments->appRootDirectory))
                return nullptr;
        } else if (currentArg == WebAppArguments::NeuralNetworkFileNameArgument) {
            if (!parseNeuralNetworkFileName(arguments->neuralNetworkFileName))
                return nullptr;
        } else if (currentArg == WebAppArguments::HighscoreThresholdArgument) {
            if (!parseHighscoreThreshold(arguments->highscoreThreshold))
                return nullptr;
        } else {
            std::cerr << "Unknown argument " << currentArg << std::endl;
            return nullptr;
        }
    }
    if (arguments->serverName.length() == 0) {
        std::cerr << "Server name not set" << std::endl;
        return nullptr;
    } else if (arguments->documentRoot.length() == 0) {
        std::cerr << "Document root not set" << std::endl;
        return nullptr;
    } else if (arguments->appRootDirectory.length() == 0) {
        std::cerr << "App root directory not set" << std::endl;
        return nullptr;
    }
    return arguments;
}

bool WebAppArgumentsParser::parsePort(unsigned short &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Port argument requires parameter" << std::endl;
        return false;
    }
    unsigned port;
    if (!tryParseUnsigned(_argv[++_currentArgIndex], port)) {
        std::cerr << "Could not parse port number " << _argv[_currentArgIndex] << std::endl;
        return false;
    } else if (port > 65535 || port < 1) {
        std::cerr << "Port number has to be greater than 0 and lesser than 65536" << std::endl;
        return false;
    }
    output = static_cast<unsigned short>(port);
    return true;
}

bool WebAppArgumentsParser::parseServerName(std::string &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Server name argument requires parameter" << std::endl;
        return false;
    }
    output = _argv[++_currentArgIndex];
    return true;
}

bool WebAppArgumentsParser::parseDocumentRoot(std::string &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Document root argument requires parameter" << std::endl;
        return false;
    }
    output = _argv[++_currentArgIndex];
    return true;
}

bool WebAppArgumentsParser::parseResourcesDirectory(std::string &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Resources directory argument requires parameter" << std::endl;
        return false;
    }
    output = _argv[++_currentArgIndex];
    return true;
}

bool WebAppArgumentsParser::parseAppRootDirectory(std::string &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "App root directory argument requires parameter" << std::endl;
        return false;
    }
    output = _argv[++_currentArgIndex];
    return true;
}

bool WebAppArgumentsParser::parseNeuralNetworkFileName(std::string &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Neural network file name argument requires parameter" << std::endl;
        return false;
    }
    output = _argv[++_currentArgIndex];
    return true;
}

bool WebAppArgumentsParser::parseHighscoreThreshold(unsigned long &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Highscore threshold argument requires parameter" << std::endl;
        return false;
    }

    unsigned threshold;

    if (!tryParseUnsigned(_argv[++_currentArgIndex], threshold)) {
        std::cerr << "Could not parse highscore threshold " << _argv[_currentArgIndex];
        return false;
    }
    output = threshold;
    return true;
}

}
