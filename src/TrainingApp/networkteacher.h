#ifndef NETWORKTEACHER_H
#define NETWORKTEACHER_H

#include "application.h"
#include <string>
#include <vector>
#include <memory>
#include <NeuralNetwork/learningnetwork.h>

namespace Stinky
{

class NetworkTeacher: public Application
{
public:
    NetworkTeacher(const std::string &networkFileName,
                   const std::string &learningSetsFileName,
                   unsigned maxEpochs,
                   double minError,
                   double learningRate,
                   double momentum);

    int run();
    void onSigInt();

protected:
    std::unique_ptr<NeuralNetwork::LearningNetwork> loadNeuralNetwork() const;
    std::vector<NeuralNetwork::LearningSet> loadLearningSets() const;
    void performLearning(NeuralNetwork::LearningNetwork *network,
                         const std::vector<NeuralNetwork::LearningSet> &learningSets) const;
    void serializeNetwork(const NeuralNetwork::LearningNetwork *network) const;

private:
    std::string _networkFileName;
    std::string _learningSetsFileName;
    unsigned _maxEpochs;
    double _minError;
    double _learningRate;
    double _momentum;
    bool _sigIntCought;
    std::unique_ptr<NeuralNetwork::LearningNetwork> _network;
};

}


#endif // NETWORKTEACHER_H
