#include "NetworkTeacherArgumentsParser.h"
#include <iostream>
#include <cmath>
#include "NetworkTeacherArguments.h"

namespace nn2048 {

NetworkTeacherArgumentsParser::NetworkTeacherArgumentsParser(int argc, char **argv):
    ArgumentParser(argc, argv, 2)
{}

std::unique_ptr<Arguments> NetworkTeacherArgumentsParser::parsedArguments()
{
    auto arguments = std::make_unique<NetworkTeacherArguments>();
    for (; _currentArgIndex < static_cast<unsigned>(_argc); ++_currentArgIndex) {
        auto currentArg = _argv[_currentArgIndex];
        if (currentArg == NetworkTeacherArguments::NetworkFileNameArgument) {
            if (!parseNetworkFileName(arguments->networkFileName))
                return nullptr;
        } else if (currentArg == NetworkTeacherArguments::LearningSetsFileNameArgument) {
            if (!parseLearningSetsFileName(arguments->learningSetsFileName))
                return nullptr;
        } else if (currentArg == NetworkTeacherArguments::MaxEpochsArgument) {
            if (!parseMaxEpochs(arguments->maxEpochs))
                return nullptr;
        } else if (currentArg == NetworkTeacherArguments::MinErrorArgument) {
            if (!parseMinError(arguments->minError))
                return nullptr;
        } else if (currentArg == NetworkTeacherArguments::LearningRateArgument) {
            if (!parseLearningRate(arguments->learningRate))
                return nullptr;
        } else if (currentArg == NetworkTeacherArguments::MomentumFactorArgument) {
            if (!parseMomentumFactor(arguments->momentum))
                return nullptr;
        } else {
            std::cerr << "Unknown argument " << currentArg << std::endl;
            return nullptr;
        }
    }
    if (arguments->networkFileName.length() == 0) {
        std::cerr << "Missing network file name argument" << std::endl;
        return nullptr;
    } else if (arguments->learningSetsFileName.length() == 0) {
        std::cerr << "Missing learning sets file name argument" << std::endl;
        return nullptr;
    } else if (arguments->maxEpochs == 0 && std::abs(arguments->minError) < 0.000001) {
        std::cerr << "Missing learning limit condition (max epochs or min error)" << std::endl;
        return nullptr;
    }
    return arguments;
}

bool NetworkTeacherArgumentsParser::parseNetworkFileName(std::string &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Network file name argument requires parameter" << std::endl;
        return false;
    }
    output = _argv[++_currentArgIndex];
    return true;
}

bool NetworkTeacherArgumentsParser::parseLearningSetsFileName(std::string &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Learning sets file name argument requires parameter" << std::endl;
        return false;
    }
    output = _argv[++_currentArgIndex];
    return true;
}

bool NetworkTeacherArgumentsParser::parseMaxEpochs(unsigned &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Max epochs argument requires parameter" << std::endl;
        return false;
    } else if (!tryParseUnsigned(_argv[++_currentArgIndex], output)) {
        std::cerr << "Could not parse max epochs parameter" << std::endl;
        return false;
    }
    return true;
}

bool NetworkTeacherArgumentsParser::parseMinError(double &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Min error argument requires parameter" << std::endl;
        return false;
    } else if (!tryParseDouble(_argv[++_currentArgIndex], output)) {
        std::cerr << "Could not parse min error parameter" << std::endl;
        return false;
    }
    return true;
}

bool NetworkTeacherArgumentsParser::parseLearningRate(double &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Learning rate argument requires parameter" << std::endl;
        return false;
    } else if (!tryParseDouble(_argv[++_currentArgIndex], output)) {
        std::cerr << "Could not parse learning rate parameter" << std::endl;
        return false;
    }
    return true;
}

bool NetworkTeacherArgumentsParser::parseMomentumFactor(double &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Momentum factor argument requires parameter" << std::endl;
        return false;
    } else if (!tryParseDouble(_argv[++_currentArgIndex], output)) {
        std::cerr << "Could not parse momentum factor parameter" << std::endl;
        return false;
    }
    return true;
}

}


