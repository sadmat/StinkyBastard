#ifndef NEURALNETWORKGAMECONTROLLER_H
#define NEURALNETWORKGAMECONTROLLER_H

#include "GameController.h"
#include <network.h>

namespace nn2048
{

class NeuralNetworkGameController : public GameController
{
public:
    NeuralNetworkGameController(GameCore *game, const NeuralNetwork::Network *network);

    void start();
    void move();

private:
    const NeuralNetwork::Network *_network;
};


}

#endif // NEURALNETWORKGAMECONTROLLER_H
