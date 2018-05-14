#include "NetworkCreatorArgumentsParser.h"
#include <iostream>
#include <sstream>
#include <string>
#include "NetworkCreatorArguments.h"

namespace nn2048 {

std::vector<std::string> splitString(const std::string &string, char delimiter);

NetworkCreatorArgumentsParser::NetworkCreatorArgumentsParser(int argc, char **argv) :
    ArgumentParser(argc, argv, 2)
{}

std::unique_ptr<Arguments> NetworkCreatorArgumentsParser::parsedArguments()
{
    auto arguments = std::make_unique<NetworkCreatorArguments>();
    for (; _currentArgIndex < static_cast<unsigned>(_argc); ++_currentArgIndex) {
        auto currentArg = _argv[_currentArgIndex];
        if (currentArg == NetworkCreatorArguments::NetworkStructureArgument) {
            if (!parseNetworkStructure(arguments->networkStructure))
                return nullptr;
        } else if (currentArg == NetworkCreatorArguments::NetworkFileNameArgument) {
            if (!parseNetworkFileName(arguments->networkFileName))
                return nullptr;
        } else if (currentArg == NetworkCreatorArguments::WeightDistributionArgument) {
            if (!parseWeightDistribution(arguments->weightDistribution))
                return nullptr;
        } else if (currentArg == NetworkCreatorArguments::CreateFannNetworkArgument) {
            if (!parseCreateFannNetwork(arguments->createFannNetwork))
                return nullptr;
        } else {
            std::cerr << "Unknown argument: " << currentArg << std::endl;
            return nullptr;
        }
    }
    if (arguments->networkFileName.length() == 0) {
        std::cerr << "Network file name not specified" << std::endl;
        return nullptr;
    } else if (arguments->networkStructure.size() == 0) {
        std::cerr << "Network structure not specified" << std::endl;
        return nullptr;
    }
    return arguments;
}

bool NetworkCreatorArgumentsParser::parseNetworkStructure(std::vector<unsigned> &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Network structure argument requires parameter" << std::endl;
        return false;
    }

    auto structure = splitString(_argv[++_currentArgIndex], ',');
    if (structure.size() == 0) {
        std::cerr << "Invalid network structure: " << _argv[_currentArgIndex] << std::endl;
        return false;
    }

    output.reserve(structure.size());
    for (auto &value: structure) {
        unsigned layerSize;
        try {
            size_t index = 0;
            layerSize = static_cast<unsigned>(std::stoul(value, &index, 10));
            if (index < value.length()) {
                std::cerr << "Could not parse structure token: " << value << std::endl;
                return false;
            }
        } catch (std::invalid_argument &ex) {
            std::cerr << "Could not parse structure token: " << value << std::endl;
            std::cerr << "Exception: " << ex.what() << std::endl;
            return false;
        }
        output.push_back(layerSize);
    }

    return true;
}

bool NetworkCreatorArgumentsParser::parseNetworkFileName(std::string &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Network file name argument requires parameter" << std::endl;
        return false;
    }
    output = _argv[++_currentArgIndex];
    return true;
}

bool NetworkCreatorArgumentsParser::parseWeightDistribution(double &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Weight distribution argument requires parameter" << std::endl;
        return false;
    } else if (!tryParseDouble(_argv[++_currentArgIndex], output)) {
        std::cerr << "Could not parse weight distribution: " << _argv[_currentArgIndex] << std::endl;
        return false;
    }
    return true;
}

bool NetworkCreatorArgumentsParser::parseCreateFannNetwork(bool &output)
{
    if (output) {
        std::cerr << "Create FANN network flag was already set" << std::endl;
        return false;
    }
    output = true;
    return true;
}

std::vector<std::string> splitString(const std::string &string, char delimiter)
{
    std::stringstream stream(string);
    std::string item;
    std::vector<std::string> elements;
    while (std::getline(stream, item, delimiter))
    {
        elements.push_back(std::move(item));
    }
    return elements;
}

}
