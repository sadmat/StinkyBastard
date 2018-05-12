#include "NetworkTeacher.h"
#include <NetworkSerializer.h>
#include <LearningSetSerializer.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <core/connection.h>

namespace nn2048
{

NetworkTeacher::NetworkTeacher(std::unique_ptr<NetworkTeacherArguments> arguments):
    _arguments(std::move(arguments)),
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

        std::ifstream file(_arguments->networkFileName);
        if (!file.is_open())
        {
            std::cout << "failed" << std::endl;
            std::cerr << "Could not open file " << _arguments->networkFileName << std::endl;
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

        std::ifstream file(_arguments->learningSetsFileName);
        if (!file.is_open())
        {
            std::cout << "failed" << std::endl;
            std::cerr << "Could not open file " << _arguments->learningSetsFileName << std::endl;
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
        network->learn(learningSets, _arguments->learningRate, _arguments->momentum, _arguments->maxEpochs, _arguments->minError, &_sigIntCought);
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

        std::ofstream file(_arguments->networkFileName);
        if (!file.is_open())
        {
            std::cout << "failed" << std::endl;
            std::cerr << "Could not open file " << _arguments->networkFileName << std::endl;
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


