#include "networkteacher.h"
#include <NeuralNetwork/networkserializer.h>
#include <NeuralNetwork/learningsetserializer.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <core/connection.h>

namespace Stinky
{

NetworkTeacher::NetworkTeacher(const std::string &networkFileName,
                               const std::string &learningSetsFileName,
                               unsigned maxEpochs,
                               double minError,
                               double learningRate,
                               double momentum):
    _networkFileName(networkFileName),
    _learningSetsFileName(learningSetsFileName),
    _maxEpochs(maxEpochs),
    _minError(minError),
    _learningRate(learningRate),
    _momentum(momentum),
    _sigIntCought(false)
{}

int NetworkTeacher::run()
{
    _network = loadNeuralNetwork();
    if (!_network)
        return -1;
    auto learningSets = loadLearningSets();
    if (!learningSets.size())
        return -1;
    performLearning(_network.get(), learningSets);
    serializeNetwork(_network.get());
    return 0;
}

void NetworkTeacher::onSigInt()
{
    std::cout << "SIGING cought" << std::endl;
    _sigIntCought = true;
}

std::unique_ptr<NeuralNetwork::LearningNetwork> NetworkTeacher::loadNeuralNetwork() const
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

std::vector<NeuralNetwork::LearningSet> NetworkTeacher::loadLearningSets() const
{
    try
    {
        std::cout << "Loading learning sets... ";
        std::cout.flush();

        std::ifstream file(_learningSetsFileName);
        if (!file.is_open())
        {
            std::cout << "failed" << std::endl;
            std::cerr << "Could not open file " << _learningSetsFileName << std::endl;
            return {};
        }

        auto learningSets = NeuralNetwork::LearningSetSerializer::deserialize(file);
        file.close();
        std::cout << "ok" << std::endl;
        return learningSets;
    }
    catch (std::runtime_error &exception)
    {
        std::cout << "failed" << std::endl;
        std::cerr << "Runtime error: " << exception.what() << std::endl;
    }
    return {};
}

void NetworkTeacher::performLearning(NeuralNetwork::LearningNetwork *network, const std::vector<NeuralNetwork::LearningSet> &learningSets) const
{
    try
    {
        auto connection = core::ScopedConnection(network->learningProgress.connect([] (unsigned epoch, double error) {
            std::cout << "Epoch finished: " << epoch << ", error: " << error << std::endl;
        }));

        std::cout << "Learning..." << std::endl;
        network->learn(learningSets, _learningRate, _momentum, _maxEpochs, _minError, &_sigIntCought);
        std::cout << "Learning finished." << std::endl;
    }
    catch (std::invalid_argument &exception)
    {
        std::cerr << "Invalid argument: " << exception.what() << std::endl;
    }
}

void NetworkTeacher::serializeNetwork(const NeuralNetwork::LearningNetwork *network) const
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

        NeuralNetwork::NetworkSerializer::serialize(network, file);
        file.close();
        std::cout << "ok" << std::endl;
    }
    catch (std::runtime_error &exception)
    {
        std::cout << "failed" << std::endl;
        std::cerr << "Runtime error: " << exception.what() << std::endl;
    }
}

}


