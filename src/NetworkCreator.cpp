#include "NetworkCreator.h"
#include <NetworkSerializer.h>
#include <fstream>
#include <iostream>

namespace nn2048
{

NetworkCreator::NetworkCreator(const std::vector<unsigned> &structure,
                               const std::string &fileName,
                               double distributionAmplitude):
    _structure(structure),
    _fileName(fileName),
    _distributionAmplitude(distributionAmplitude)
{}

int NetworkCreator::run()
{
    if (_structure.size() < 2)
    {
        std::cerr << "Network structure has to have at least two values (input count, neuron count)." << std::endl;
        return 0;
    }

    std::cout << "Creating network... ";
    auto network = createNetwork();
    if (!network)
    {
        std::cout << "failed" << std::endl;
        return 0;
    }
    else std::cout << "ok" << std::endl;

    serialize(network.get());
    return 0;
}

std::unique_ptr<NeuralNetwork::Network> NetworkCreator::createNetwork() const
{
    NeuralNetwork::LayerVector layers = createLayers();
    return std::make_unique<NeuralNetwork::Network>(std::move(layers));
}

NeuralNetwork::LayerVector NetworkCreator::createLayers() const
{
    NeuralNetwork::LayerVector layers;
    layers.reserve(_structure.size() - 1);
    for (unsigned i = 1; i < _structure.size(); ++i)
    {
        auto layer = createLayer(_structure[i - 1], _structure[i]);
        layers.push_back(std::move(layer));
    }
    return layers;
}

std::unique_ptr<NeuralNetwork::Layer> NetworkCreator::createLayer(unsigned inputCount, unsigned neuronCount) const
{
    auto neurons = createNeurons(inputCount, neuronCount);
    return std::make_unique<NeuralNetwork::Layer>(std::move(neurons));
}

NeuralNetwork::NeuronVector NetworkCreator::createNeurons(unsigned inputCount, unsigned neuronCount) const
{
    NeuralNetwork::NeuronVector neurons;
    for (unsigned i = 0; i < neuronCount; ++i)
    {
        auto neuron = std::make_unique<NeuralNetwork::Neuron>(inputCount);
        neuron->randomize(_distributionAmplitude);
        neurons.push_back(std::move(neuron));
    }
    return neurons;
}

void NetworkCreator::serialize(const NeuralNetwork::Network *network) const
{
    try
    {
        std::cout << "Serializing...";
        std::cout.flush();

        std::ofstream file(_fileName);
        if (!file.is_open())
        {
            std::cout << "failed" << std::endl;
            std::cerr << "Could not open file " << _fileName << std::endl;
            return;
        }

        NeuralNetwork::NetworkSerializer::serialize(network, file);
        file.close();
        std::cout << "ok" << std::endl;
    }
    catch (std::runtime_error &exception)
    {
        std::cout << "failed" << std::endl;
        std::cerr << "Runtime exception: " << exception.what() << std::endl;
    }
}

}


