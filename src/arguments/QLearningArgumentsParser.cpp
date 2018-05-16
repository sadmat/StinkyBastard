#include "QLearningArgumentsParser.h"
#include <iostream>
#include "QLearningArguments.h"

namespace nn2048 {

QLearningArgumentsParser::QLearningArgumentsParser(int argc, char **argv) :
    ArgumentParser(argc, argv, 2)
{}

std::unique_ptr<Arguments> QLearningArgumentsParser::parsedArguments()
{
    auto arguments = std::make_unique<QLearningArguments>();
    for (; _currentArgIndex < static_cast<unsigned>(_argc); ++_currentArgIndex) {
        auto currentArg = _argv[_currentArgIndex];
        if (currentArg == QLearningArguments::NetworkFileNameArgument) {
            if (!parseNetworkFileName(arguments->networkFileName))
                return nullptr;
        } else if (currentArg == QLearningArguments::MaxAgeArgument) {
            if (!parseMaxAge(arguments->maxAge))
                return nullptr;
        } else if (currentArg == QLearningArguments::TargetScoreArgument) {
            if (!parseTargetScore(arguments->targetScore))
                return nullptr;
        } else if (currentArg == QLearningArguments::GammaFactorArgument) {
            if (!parseGammaFactor(arguments->gamma))
                return nullptr;
        } else if (currentArg == QLearningArguments::LearningRateArgument) {
            if (!parseLearningRate(arguments->learningRate))
                return nullptr;
        } else if (currentArg == QLearningArguments::MomentumFactorArgument) {
            if (!parseMomentumFactor(arguments->momentumFactor))
                return nullptr;
        } else if (currentArg == QLearningArguments::EpsilonFactorArgument) {
            if (!parseEpsilonFactor(arguments->epsilonFactor))
                return nullptr;
        } else if (currentArg == QLearningArguments::ReplayMemorySizeArgument) {
            if (!parseReplayMemorySize(arguments->replayMemorySize))
                return nullptr;
        } else if (currentArg == QLearningArguments::ReplayBatchSizeArgument) {
            if (!parseReplayBatchSize(arguments->replayBatchSize))
                return nullptr;
        } else if (currentArg == QLearningArguments::ReplayMemoryFileNameArgument) {
            if (!parseReplayMemoryFileName(arguments->replayMemoryFileName))
                return nullptr;
        } else {
            std::cerr << "Unknown qlearning argument: " << currentArg << std::endl;
            return nullptr;
        }
    }
    if (arguments->networkFileName.length() == 0) {
        std::cerr << "Network file name not specified" << std::endl;
        return nullptr;
    } else if (arguments->maxAge == 0 && arguments->targetScore == 0) {
        std::cerr << "Max age or target score parameters have to be set" << std::endl;
        return nullptr;
    }
    return arguments;
}

bool QLearningArgumentsParser::parseNetworkFileName(std::string &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Network file name not specified" << std::endl;
        return false;
    }
    output = _argv[++_currentArgIndex];
    return true;
}

bool QLearningArgumentsParser::parseMaxAge(unsigned &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Max age requires parameter" << std::endl;
        return false;
    } else if (!tryParseUnsigned(_argv[++_currentArgIndex], output)) {
        std::cerr << "Could not parse max age parameter" << std::endl;
        return false;
    }
    return true;
}

bool QLearningArgumentsParser::parseTargetScore(unsigned &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Target score argument requires parameter" << std::endl;
        return false;
    } else if (!tryParseUnsigned(_argv[++_currentArgIndex], output)) {
        std::cerr << "Could not parse target score" << std::endl;
        return false;
    }
    return true;
}

bool QLearningArgumentsParser::parseGammaFactor(double &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Gamma factor argument requires parameter" << std::endl;
        return false;
    } else if (!tryParseDouble(_argv[++_currentArgIndex], output)) {
        std::cerr << "Could not parse gamma factor" << std::endl;
        return false;
    }
    return true;
}

bool QLearningArgumentsParser::parseLearningRate(double &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Learning rate argument requires parameter" << std::endl;
        return false;
    } else if (!tryParseDouble(_argv[++_currentArgIndex], output)) {
        std::cerr << "Could not parse learning rate" << std::endl;
        return false;
    }
    return true;
}

bool QLearningArgumentsParser::parseMomentumFactor(double &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Momentum factor argument requires parameter" << std::endl;
        return false;
    } else if (!tryParseDouble(_argv[++_currentArgIndex], output)) {
        std::cerr << "Could not parse momentum factor" << std::endl;
        return false;
    }
    return true;
}

bool QLearningArgumentsParser::parseEpsilonFactor(double &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Epsilon factor argument requires parameter" << std::endl;
        return false;
    } else if (!tryParseDouble(_argv[++_currentArgIndex], output)) {
        std::cerr << "Could not parse epsilon factor" << std::endl;
        return false;
    }
    return true;
}

bool QLearningArgumentsParser::parseReplayMemorySize(unsigned &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Replay memory size argument requires parameter" << std::endl;
        return false;
    } else if (!tryParseUnsigned(_argv[++_currentArgIndex], output)) {
        std::cerr << "Could not parse replay memory size" << std::endl;
        return false;
    }
    return true;
}

bool QLearningArgumentsParser::parseReplayBatchSize(unsigned &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Replay batch size argument requires parameter" << std::endl;
        return false;
    } else if (!tryParseUnsigned(_argv[++_currentArgIndex], output)) {
        std::cerr << "Could not parse replay batch size" << std::endl;
        return false;
    }
    return true;
}

bool QLearningArgumentsParser::parseReplayMemoryFileName(std::string &output)
{
    if (!hasParameter(_currentArgIndex)) {
        std::cerr << "Replay memory file name argument requires parameter" << std::endl;
        return false;
    }
    output = _argv[++_currentArgIndex];
    return true;
}



}
