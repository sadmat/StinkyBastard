#ifndef NETWORKTEACHER_H
#define NETWORKTEACHER_H

#include "Application.h"
#include <string>
#include <vector>
#include <memory>
#include <LearningNetwork.h>
#include "arguments/NetworkTeacherArguments.h"

namespace nn2048
{

class NetworkTeacher: public Application
{
public:
    NetworkTeacher(std::unique_ptr<NetworkTeacherArguments> arguments);

    int run();
    void onSigInt();

protected:
    std::unique_ptr<NeuralNetwork::LearningNetwork> loadNeuralNetwork() const;
    std::vector<NeuralNetwork::LearningSet> loadLearningSets() const;
    void performLearning(NeuralNetwork::LearningNetwork *network,
                         const std::vector<NeuralNetwork::LearningSet> &learningSets) const;
    void serializeNetwork(const NeuralNetwork::LearningNetwork *network) const;

private:
    std::unique_ptr<NetworkTeacherArguments> _arguments;
    bool _sigIntCought;
    std::unique_ptr<NeuralNetwork::LearningNetwork> _network;
};

}


#endif // NETWORKTEACHER_H
