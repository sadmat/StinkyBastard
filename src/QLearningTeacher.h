//
// Created by Mateusz Sądel on 04.03.2018.
//

#ifndef NN2048_QLEARNINGTEACHER_H
#define NN2048_QLEARNINGTEACHER_H

#include "Application.h"
#include <string>
#include <vector>
#include <memory>
#include <GameCore.h>
#include <doublefann.h>
#include <fann_cpp.h>

namespace nn2048
{

class QLearningState;

class QLearningTeacher: public Application
{
public:
    QLearningTeacher(const std::string &networkFileName,
                         unsigned maxAge,
                         unsigned targetScore,
                         double gamma,
                         double learningRate,
                         double momentum);

    int run();
    void onSigInt();

protected:
    std::unique_ptr<FANN::neural_net> loadNeuralNetwork() const;
    std::unique_ptr<FANN::neural_net> createNeuralNetwork() const;
    void performLearning() const;
    FANN::training_data prepareTrainingData(const std::vector<const QLearningState *> &batch) const;
    void serializeNetwork() const;
    double computeReward(bool moveFailed, unsigned deltaScore) const;
    std::function<bool()> learningCondition(const unsigned &age, const unsigned &score) const;
    void printStats(unsigned epoch, unsigned score, unsigned steps, unsigned illegalSteps) const;

private:
    std::string _networkFileName;
    unsigned _maxAge;
    unsigned _targetScore;
    double _gamma;
    double _learningRate;
    double _momentum;
    bool _sigIntCaught;
    std::unique_ptr<FANN::neural_net> _network;
    std::unique_ptr<Game2048Core::GameCore> _game;
};

}




#endif //NN2048_QLEARNINGTEACHER_H
