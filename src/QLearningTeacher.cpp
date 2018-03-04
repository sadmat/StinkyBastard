//
// Created by Mateusz Sądel on 04.03.2018.
//

#include "QLearningTeacher.h"
#include <NetworkSerializer.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "utils/BoardSignalConverter.h"
#include "utils/NetworkOutputConverter.h"

namespace nn2048
{

const unsigned gameBoardSideLength = 4;

QLearningTeacher::QLearningTeacher(const std::string &networkFileName,
                                   unsigned maxEpochs,
                                   double gamma,
                                   double learningRate,
                                   double momentum):
_networkFileName(networkFileName),
_maxEpochs(maxEpochs),
_gamma(gamma),
_learningRate(learningRate),
_momentum(momentum),
_network(nullptr),
_game(std::make_unique<Game2048Core::GameCore>(gameBoardSideLength))
{}

int QLearningTeacher::run()
{
    _network = loadNeuralNetwork();
    if (!_network)
        return -1;
    performLearning();
    serializeNetwork();
    return 0;
}

void QLearningTeacher::onSigInt()
{
    _sigIntCaught = true;
}

std::unique_ptr<NeuralNetwork::LearningNetwork> QLearningTeacher::loadNeuralNetwork() const
{
    try
    {
        std::cout << "Loading neural network... ";
        std::cout.flush();

        std::ifstream file(_networkFileName);
        if (!file.is_open())
        {
            std::cout << "failed" << std::endl;
            std::cerr << "Could not open file " << _networkFileName << std::endl;
            return nullptr;
        }

        auto network = NeuralNetwork::NetworkSerializer::deserialize(file);
        file.close();
        std::cout << "ok" << std::endl;
        return std::make_unique<NeuralNetwork::LearningNetwork>(std::move(network));
    }
    catch (std::runtime_error &exception)
    {
        std::cout << "failed" << std::endl;
        std::cerr << "Runtime exception: " << exception.what() << std::endl;
    }
    return nullptr;
}

void QLearningTeacher::performLearning() const
{
    try
    {
        unsigned agentStepCount = 0;
        for (int i = 0; i < _maxEpochs && !_sigIntCaught; ++i)
        {
            if (_game->isGameOver())
            {
                std::cout << "[epoch: " << i + 1 << "] Game over with agent steps: " << agentStepCount << ", score: " << _game->score() << std::endl;
                _game->reset();
                agentStepCount = 0;
            }

            auto currentStateSignal = BoardSignalConverter::boardToSignal(_game->board());
            auto qValues = NetworkOutputConverter::outputToMoves(_network->responses(currentStateSignal));
            bool moveFailed = !_game->tryMove(qValues.front().first);
            double reward = computeReward(moveFailed);
            auto newStateSignal = BoardSignalConverter::boardToSignal(_game->board());
            _network->performQLearning((unsigned)qValues.front().first,
                                       reward,
                                       currentStateSignal,
                                       newStateSignal,
                                       _learningRate,
                                       _momentum,
                                       _gamma);
        }
    }
    catch (std::invalid_argument &exception)
    {
        std::cerr << "Invalid argument: " << exception.what() << std::endl;
    }

}

void QLearningTeacher::serializeNetwork() const
{
    try
    {
        std::cout << "Serializing network... ";
        std::cout.flush();

        std::ofstream file(_networkFileName);
        if (!file.is_open())
        {
            std::cout << "failed" << std::endl;
            std::cerr << "Could not open file " << _networkFileName << std::endl;
            return;
        }

        NeuralNetwork::NetworkSerializer::serialize(_network.get(), file);
        file.close();
        std::cout << "ok" << std::endl;
    }
    catch (std::runtime_error &exception)
    {
        std::cout << "failed" << std::endl;
        std::cerr << "Runtime error: " << exception.what() << std::endl;
    }
}

double QLearningTeacher::computeReward(bool moveFailed) const
{
    return 0;
}

}