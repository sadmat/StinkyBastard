#include "NeuralNetworkGameController.h"
#include <Wt/WTimer.h>
#include <iostream>
#include <chrono>
#include <functional>
#include <map>
#include "../utils/BoardSignalConverter.h"
#include "../utils/NetworkOutputConverter.h"

namespace nn2048
{

NeuralNetworkGameController::NeuralNetworkGameController(GameCore *gameCore,
                                                         const NeuralNetwork::Network *network,
                                                         bool autoRestart):
    GameController(gameCore),
    _network(network),
    _autoRestart(autoRestart)
{}

void NeuralNetworkGameController::start()
{
    std::clog << "NeuralNetworkGameController: starting" << std::endl;
    Wt::WTimer::singleShot(std::chrono::milliseconds(500), [this] (){
        move();
    });
}

void NeuralNetworkGameController::move()
{
    static std::map<Direction, std::string> directionDictionary {
        { Direction::Up, "up" },
        { Direction::Down, "down" },
        { Direction::Left, "left" },
        { Direction::Right, "right" }
    };

    std::clog << "NeuralNetworkGameController::move()" << std::endl;

    if (_gameCore->isGameOver())
    {
        std::clog << "Game over." << std::endl;
        if (_autoRestart)
        {
            std::clog <<  "Restarting." << std::endl;
            _gameCore->reset();
            start();
        }
        return;
    }

    auto boardSignal = BoardSignalConverter::boardToSignal(_gameCore->board());
    auto response = _network->responses(boardSignal);
    auto directions = NetworkOutputConverter::outputToMoves(response);
    for (auto direction: directions)
    {
        std::clog << "Trying direction " << directionDictionary[direction.first] << " (" << direction.second << ")... ";
        if (_gameCore->tryMove(direction.first))
        {
            std::clog << "ok" << std::endl;
            start();
            break;
        }
        else
            std::clog << "failed" << std::endl;
    }
}

}

