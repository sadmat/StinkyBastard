#include "Reinforcement.h"

namespace nn2048 {

double Reinforcement::computeReinforcement(bool gameOver, bool moveSucceeded, unsigned score, unsigned prevScore)
{
    if (gameOver)
        return -2.0;
    else if (!moveSucceeded)
        return -1.0;
    else if (score > prevScore) {
        if (prevScore == 0)
            return 1.0;
        return static_cast<double>(score) / (2.0 * prevScore);
    }
    return 0.0;
}

}


