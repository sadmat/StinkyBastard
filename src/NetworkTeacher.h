#ifndef NETWORKTEACHER_H
#define NETWORKTEACHER_H

#include "Application.h"
#include <memory>
#include <map>
#include <vector>
#include <doublefann.h>
#include <fann_cpp.h>
#include "arguments/NetworkTeacherArguments.h"
#include "utils/ReplayMemory.h"

namespace nn2048
{

class NetworkTeacher: public Application
{
public:
    NetworkTeacher(std::unique_ptr<NetworkTeacherArguments> arguments);

    int run();
    void onSigInt();

protected:
    bool initialize();
    std::unique_ptr<FANN::neural_net> loadNeuralNetwork();
    std::unique_ptr<ReplayMemory> loadReplayMemory();
    std::vector<std::string> replayMemoryFileNames();
    void performTraining();
    bool serializeNetwork();

private:
    std::unique_ptr<NetworkTeacherArguments> _arguments;
    bool _sigIntCought;
    std::unique_ptr<FANN::neural_net> _network;
    std::unique_ptr<ReplayMemory> _replayMemory;
    std::map<QLearningState *, double> _qvalueCache;
};

}


#endif // NETWORKTEACHER_H
