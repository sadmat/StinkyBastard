//
// Created by Mateusz Sądel on 04.03.2018.
//

#ifndef NN2048_QLEARNINGTEACHER_H
#define NN2048_QLEARNINGTEACHER_H

#include "Application.h"
#include <string>
#include <vector>
#include <memory>
#include <LearningNetwork.h>
#include <GameCore.h>

namespace nn2048
{

class QLearningTeacher: public Application
{
public:
    QLearningTeacher(const std::string &networkFileName,
                     unsigned maxEpochs,
                     double gamma,
                     double learningRate,
                     double momentum);

    int run();
    void onSigInt();

protected:
    std::unique_ptr<NeuralNetwork::LearningNetwork> loadNeuralNetwork() const;
    void performLearning() const;
    void serializeNetwork() const;
    double computeReward(bool moveFailed) const;

private:
    std::string _networkFileName;
    unsigned _maxEpochs;
    double _gamma;
    double _learningRate;
    double _momentum;
    bool _sigIntCaught;
    std::unique_ptr<NeuralNetwork::LearningNetwork> _network;
    std::unique_ptr<Game2048Core::GameCore> _game;
};

}




#endif //NN2048_QLEARNINGTEACHER_H
