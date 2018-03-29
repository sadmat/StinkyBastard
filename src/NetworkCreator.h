#ifndef NETWORKCREATOR_H
#define NETWORKCREATOR_H

#include "Application.h"
#include <vector>
#include <string>
#include <memory>
#include <Network.h>
#include <doublefann.h>
#include <fann_cpp.h>
#include "arguments/NetworkCreatorArguments.h"

namespace nn2048
{

class NetworkCreator: public Application
{
public:
    NetworkCreator(std::unique_ptr<NetworkCreatorArguments> arguments);

    int run();

protected:
    std::unique_ptr<NeuralNetwork::Network> createNetwork() const;
    NeuralNetwork::LayerVector createLayers() const;
    std::unique_ptr<NeuralNetwork::Layer> createLayer(unsigned inputCount, unsigned neuronCount) const;
    NeuralNetwork::NeuronVector createNeurons(unsigned inputCount, unsigned neuronCount) const;
    void serialize(const NeuralNetwork::Network *network) const;

    std::unique_ptr<FANN::neural_net> createFann() const;
    void serializeFann(FANN::neural_net *network) const;

private:
    std::unique_ptr<NetworkCreatorArguments> _arguments;
};

}


#endif // NETWORKCREATOR_H
