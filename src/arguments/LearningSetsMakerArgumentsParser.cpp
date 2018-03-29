#include "LearningSetsMakerArgumentsParser.h"
#include <iostream>
#include "LearningSetsMakerArguments.h"

namespace nn2048 {

LearningSetsMakerArgumentsParser::LearningSetsMakerArgumentsParser(int argc, char **argv) :
    ArgumentParser(argc, argv, 2)
{}

std::unique_ptr<Arguments> LearningSetsMakerArgumentsParser::parsedArguments()
{
    auto arguments = std::make_unique<LearningSetsMakerArguments>();

    for (; _currentArgIndex < static_cast<unsigned>(_argc); ++_currentArgIndex) {
        auto currentArg = _argv[_currentArgIndex];
        if (currentArg == LearningSetsMakerArguments::RecordsDirectoryArgument) {
            if (!parseRecordsDirectory(arguments->recordsDirectory))
                return nullptr;
        } else if (currentArg == LearningSetsMakerArguments::OutputFileNameArgument) {
            if (!parseOutputFileName(arguments->outputFileName))
                return nullptr;
        } else if (currentArg == LearningSetsMakerArguments::MinScoreArgument) {
            if (!parseMinimumScore(arguments->minScore))
                return nullptr;
        } else {
            std::cerr << "Unknown convert argument " << currentArg << std::endl;
            return nullptr;
        }
    }

    if (arguments->recordsDirectory.length() == 0) {
        std::cerr << "Records directory not specified" << std::endl;
        return nullptr;
    } else if (arguments->outputFileName.length() == 0) {
        std::cerr << "Output file name not specified" << std::endl;
        return nullptr;
    }
    return arguments;
}

bool LearningSetsMakerArgumentsParser::parseRecordsDirectory(std::string &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Records directory argument requires parameter" << std::endl;
        return false;
    }
    output = _argv[++_currentArgIndex];
    return true;
}

bool LearningSetsMakerArgumentsParser::parseOutputFileName(std::string &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Output file name argument requires parameter" << std::endl;
        return false;
    }
    output = _argv[++_currentArgIndex];
    return true;
}

bool LearningSetsMakerArgumentsParser::parseMinimumScore(unsigned &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Minimum score argument requires parameter" << std::endl;
        return false;
    } else if (!tryParseUnsigned(_argv[++_currentArgIndex], output)) {
        std::cerr << "Could not parse minimum score argument: " << _argv[_currentArgIndex] << std::endl;
        return false;
    }
    return true;
}



}
