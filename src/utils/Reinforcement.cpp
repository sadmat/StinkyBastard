#include "Reinforcement.h"
#include <cmath>
#include "BoardSignalConverter.h"

namespace nn2048 {

double Reinforcement::computeReinforcement(Game2048Core::GameCore *gameCore, bool moveSucceeded, unsigned deltaScore)
{
    if (gameCore->isGameOver())
        return -2.0;
    else if (!moveSucceeded)
        return -1.0;
    else if (deltaScore > 0) {
        auto maxTileValue = BoardSignalConverter::maxTileValue(gameCore->board());
        return std::log2(deltaScore) / std::log2(maxTileValue) + 1.0;
    }
    return 0.0;
}

}


