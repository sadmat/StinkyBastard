#ifndef REINFORCEMENT_H
#define REINFORCEMENT_H

#include <GameCore.h>

namespace nn2048 {

class Reinforcement
{
public:
    static double computeReinforcement(Game2048Core::GameCore *gameCore, bool moveSucceeded, unsigned deltaScore);
};

}


#endif // REINFORCEMENT_H
