//
// Created by Mateusz Sądel on 04.03.2018.
//

#include "QLearningTeacher.h"
#include <iostream>
#include <stdexcept>
#include <boost/filesystem.hpp>
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
    if (boost::filesystem::exists(_networkFileName))
        _network = loadNeuralNetwork();
    else
    {
        std::cout << "Neural network not found. Creating new one... ";
        _network = createNeuralNetwork();
        std::cout << "ok" << std::endl;
    }
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

std::unique_ptr<FANN::neural_net> QLearningTeacher::loadNeuralNetwork() const
{
    try
    {
        std::cout << "Loading neural network... ";
        std::cout.flush();

        auto network = std::make_unique<FANN::neural_net>(_networkFileName);

        std::cout << "ok" << std::endl;
        return network;
    }
    catch (std::runtime_error &exception)
    {
        std::cout << "failed" << std::endl;
        std::cerr << "Runtime exception: " << exception.what() << std::endl;
    }
    return nullptr;
}

std::unique_ptr<FANN::neural_net> QLearningTeacher::createNeuralNetwork() const
{
    const int networkType = FANN::LAYER;
    const unsigned int layerCount = 4;
    const unsigned int neuronCounts[] = { 16, 256, 128, 4 };
    auto network = std::make_unique<FANN::neural_net>(networkType, layerCount, neuronCounts);

    network->set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);
    network->set_activation_function_output(FANN::LINEAR);
    network->set_learning_rate(static_cast<float>(_learningRate));
    network->set_learning_momentum(static_cast<float>(_momentum));
    network->randomize_weights(-0.1f, 0.1f);

    return network;
}

void QLearningTeacher::performLearning() const
{
}

void QLearningTeacher::serializeNetwork() const
{
    std::cout << "Serializing network... ";
    _network->save(_networkFileName);
    std::cout << "ok" << std::endl;
}

double QLearningTeacher::computeReward(bool moveFailed, bool scoreIncreased) const
{
    if (moveFailed)
        return -1.0;
    else if (_game->isGameOver())
        return -1.0;
    else if (scoreIncreased)
        return 1.0; // TODO: should reward depend on score?
    else
        return 0.1; // Reward for survival.
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

void QLearningTeacher::printStats(unsigned epoch, unsigned score, unsigned steps, unsigned illegalSteps) const
{
    std::cout << "[epoch:\t" << epoch
              << "] score:\t" << score
              << ", steps:\t" << steps
              << ", illegal steps:\t" << illegalSteps
              << " (" << (double(illegalSteps)/steps) * 100 << "%)" << std::endl;
}

}
