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
#include "arguments/QLearningArguments.h"
#include "utils/ReplayMemory.h"

namespace nn2048
{

class QLearningState;

class QLearningTeacher: public Application
{
public:
    QLearningTeacher(std::unique_ptr<QLearningArguments> arguments);

    int run();
    void onSigInt();

protected:
    std::unique_ptr<FANN::neural_net> loadNeuralNetwork() const;
    std::unique_ptr<ReplayMemory> loadReplayMemory() const;
    void performLearning() const;
    double trainNetwork(const std::vector<const QLearningState *> &batch) const;
    void serializeNetwork() const;
    std::function<bool()> learningCondition(const unsigned &age, const unsigned &score) const;
    void printStats(unsigned epoch, unsigned score, unsigned steps, unsigned illegalSteps, double loss, double currentLoss) const;

private:
    std::unique_ptr<QLearningArguments> _arguments;
    bool _sigIntCaught;
    std::unique_ptr<FANN::neural_net> _network;
    std::unique_ptr<Game2048Core::GameCore> _game;
    std::unique_ptr<ReplayMemory> _replayMemory;
};

}




#endif //NN2048_QLEARNINGTEACHER_H
