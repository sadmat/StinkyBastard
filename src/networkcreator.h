#ifndef NETWORKCREATOR_H
#define NETWORKCREATOR_H

#include "application.h"
#include <vector>
#include <string>
#include <memory>
#include <network.h>

namespace Stinky
{

class NetworkCreator: public Application
{
public:
    NetworkCreator(const std::vector<unsigned> &structure,
                   const std::string &fileName,
                   double distributionAmplitude);

    int run();

protected:
    std::unique_ptr<NeuralNetwork::Network> createNetwork() const;
    NeuralNetwork::LayerVector createLayers() const;
    std::unique_ptr<NeuralNetwork::Layer> createLayer(unsigned inputCount, unsigned neuronCount) const;
    NeuralNetwork::NeuronVector createNeurons(unsigned inputCount, unsigned neuronCount) const;
    void serialize(const NeuralNetwork::Network *network) const;

private:
    std::vector<unsigned> _structure;
    std::string _fileName;
    double _distributionAmplitude;
};

}


#endif // NETWORKCREATOR_H
