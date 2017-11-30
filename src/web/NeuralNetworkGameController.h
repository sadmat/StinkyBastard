#ifndef NEURALNETWORKGAMECONTROLLER_H
#define NEURALNETWORKGAMECONTROLLER_H

#include "GameController.h"
#include <Network.h>

namespace nn2048
{

class NeuralNetworkGameController : public GameController
{
public:
    NeuralNetworkGameController(GameCore *game,
                                const NeuralNetwork::Network *network,
                                bool autoRestart);

    void start();
    void move();

private:
    const NeuralNetwork::Network *_network;
    bool _autoRestart;
};


}

#endif // NEURALNETWORKGAMECONTROLLER_H
