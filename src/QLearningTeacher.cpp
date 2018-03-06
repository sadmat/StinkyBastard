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
                                   unsigned targetScore,
                                   double gamma,
                                   double learningRate,
                                   double momentum)
    :
_networkFileName(networkFileName),
_maxEpochs(maxEpochs),
_targetScore(targetScore),
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
        std::cout << "Learning starts..." << std::endl;
        unsigned epoch = 0;
        unsigned agentStepCount = 0;
        unsigned illegalMoveCount = 0;
        auto condition = learningCondition(epoch, _game->state().score);
        for (; condition() && !_sigIntCaught; ++epoch)
        {
            if (_game->isGameOver())
            {
                std::cout << "[epoch: " << epoch + 1
                          << "] Game over with agent steps: " << agentStepCount
                          << ", illegal movements: " << illegalMoveCount
                          << ", score: " << _game->score() << std::endl;
                _game->reset();
                agentStepCount = 0;
                illegalMoveCount = 0;
            }
            else if ((agentStepCount + 1 ) % 1000 == 0)
            {
                std::cout << "[epoch: " << epoch + 1
                          << "] agent steps: " << agentStepCount
                          << ", illegal movements: " << illegalMoveCount
                          << ", score: " << _game->score() << std::endl;
            }

            auto currentStateSignal = BoardSignalConverter::boardToSignal(_game->board());
            auto qValues = NetworkOutputConverter::outputToMoves(_network->responses(currentStateSignal));
            unsigned prevScore = _game->score();
            bool moveFailed = !_game->tryMove(qValues.front().first);
            double reward = computeReward(moveFailed, prevScore < _game->score());
            auto newStateSignal = BoardSignalConverter::boardToSignal(_game->board());
            _network->performQLearning((unsigned)qValues.front().first,
                                       reward,
                                       currentStateSignal,
                                       newStateSignal,
                                       _learningRate,
                                       _momentum,
                                       _gamma);
            if (moveFailed)
                ++illegalMoveCount;
            ++agentStepCount;
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

double QLearningTeacher::computeReward(bool moveFailed, bool scoreIncreased) const
{
    if (moveFailed)
        return -0.5;
    else if (_game->isGameOver())
        return -1.0;
    else if (scoreIncreased)
        return 1.0; // TODO: should reward depend on score?
    else
        return 0.2; // Reward for survival.
}

std::function<bool()> QLearningTeacher::learningCondition(const unsigned &epoch, const unsigned &score) const
{
    if (_maxEpochs > 0 && _targetScore > 0)
    {
        auto c = [&epoch, &score, this] () -> bool {
            return epoch < this->_maxEpochs && score < this->_targetScore;
        };
        return std::bind(c);
    }
    else if (_maxEpochs > 0)
    {
        auto c = [&epoch, this] () -> bool {
            return epoch < this->_maxEpochs;
        };
        return std::bind(c);
    }
    else if (_targetScore > 0)
    {
        auto c= [&score, this] () -> bool {
            return score < _targetScore;
        };
        return std::bind(c);
    }
    return std::bind([] () -> bool { return false; });
}

}
