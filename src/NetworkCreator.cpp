#include "NetworkCreator.h"
#include <NetworkSerializer.h>
#include <fstream>
#include <iostream>

namespace nn2048
{

NetworkCreator::NetworkCreator(const std::vector<unsigned> &structure,
                               const std::string &fileName,
                               double distributionAmplitude,
                               bool fannNetwork):
    _structure(structure),
    _fileName(fileName),
    _distributionAmplitude(distributionAmplitude),
    _fannNetwork(fannNetwork)
{}

int NetworkCreator::run()
{
    if (_structure.size() < 2)
    {
        std::cerr << "Network structure has to have at least two values (input count, neuron count)." << std::endl;
        return 0;
    }

    if (!_fannNetwork)
    {
        std::cout << "Creating network... ";
        auto network = createNetwork();
        if (!network)
        {
            std::cout << "failed" << std::endl;
            return 0;
        }
        else std::cout << "ok" << std::endl;

        serialize(network.get());
    }
    else
    {
        std::cout << "Creating network...";
        auto network = createFann();
        if (!network)
        {
            std::cout << "failed" << std::endl;
            return 0;
        }
        else std::cout << "ok" << std::endl;

        serializeFann(network.get());
    }

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

std::unique_ptr<FANN::neural_net> NetworkCreator::createFann() const
{
    FANN::network_type_enum networkType = FANN::LAYER;
    unsigned layerCount = static_cast<unsigned>(_structure.size());
    const unsigned *layerSizes = &_structure[0];
    auto network = std::make_unique<FANN::neural_net>(networkType, layerCount, layerSizes);

    network->randomize_weights(-_distributionAmplitude / 2.0, _distributionAmplitude / 2.0);
    network->set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);
    network->set_activation_function_output(FANN::LINEAR);

    return network;
}

void NetworkCreator::serializeFann(FANN::neural_net *network) const
{
    std::cout << "Serializing... ";
    if (network->save(_fileName))
        std::cout << "ok" << std::endl;
    else
        std::cout << "failed" << std::endl;
}

}


