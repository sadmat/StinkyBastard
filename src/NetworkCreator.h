#ifndef NETWORKCREATOR_H
#define NETWORKCREATOR_H

#include "Application.h"
#include <vector>
#include <string>
#include <memory>
#include <Network.h>
#include <doublefann.h>
#include <fann_cpp.h>

namespace nn2048
{

class NetworkCreator: public Application
{
public:
    NetworkCreator(const std::vector<unsigned> &structure,
                   const std::string &fileName,
                   double distributionAmplitude,
                   bool fannNetwork = false);

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
    std::vector<unsigned> _structure;
    std::string _fileName;
    double _distributionAmplitude;
    bool _fannNetwork;
};

}


#endif // NETWORKCREATOR_H
