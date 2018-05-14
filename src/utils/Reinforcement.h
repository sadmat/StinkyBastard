#ifndef REINFORCEMENT_H
#define REINFORCEMENT_H

#include <GameCore.h>

namespace nn2048 {

class Reinforcement
{
public:
    static double computeReinforcement(bool gameOver, bool moveSucceeded, unsigned score, unsigned prevScore);
};

}


#endif // REINFORCEMENT_H
