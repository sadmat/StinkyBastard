#include "ReplayMemoryMergerArgumentsParser.h"
#include <iostream>
#include "ReplayMemoryMergerArguments.h"

namespace nn2048 {

ReplayMemoryMergerArgumentsParser::ReplayMemoryMergerArgumentsParser(int argc, char **argv) :
    ArgumentParser(argc, argv, 2)
{ }

std::unique_ptr<Arguments> ReplayMemoryMergerArgumentsParser::parsedArguments()
{
    auto arguments = std::make_unique<ReplayMemoryMergerArguments>();
    for (; _currentArgIndex < static_cast<unsigned>(_argc); ++_currentArgIndex) {
        auto currentArg = _argv[_currentArgIndex];
        if (currentArg == ReplayMemoryMergerArguments::InputDirectoryArgument) {
            if (!parseInputDirectory(arguments->inputDirectory))
                return nullptr;
        } else if (currentArg == ReplayMemoryMergerArguments::OutputFileNameArgument) {
            if (!parseOutputFileName(arguments->outputFileName))
                return nullptr;
        } else {
            std::cerr << "Unknown argument " << currentArg << std::endl;
            return nullptr;
        }
    }
    if (arguments->inputDirectory.empty()) {
        std::cerr << "Missing input directory argument" << std::endl;
        return nullptr;
    } else if (arguments->outputFileName.empty()) {
        std::cerr << "Missing output file name argument" << std::endl;
        return nullptr;
    }
    return arguments;
}

bool ReplayMemoryMergerArgumentsParser::parseInputDirectory(std::string &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Input directory argument requires parameter" << std::endl;
        return false;
    }
    output = _argv[++_currentArgIndex];
    return true;
}

bool ReplayMemoryMergerArgumentsParser::parseOutputFileName(std::string &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Output file name argument requires parameter" << std::endl;
        return false;
    }
    output = _argv[++_currentArgIndex];
    return true;
}



}
