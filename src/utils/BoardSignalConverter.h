#ifndef BOARDSIGNALCONVERTER_H
#define BOARDSIGNALCONVERTER_H

#include <GameCore.h>

namespace nn2048
{

class BoardSignalConverter
{
public:
    const static unsigned numberOfTiles = 16;
    const static unsigned numberOfPossibleValues = 16;
    const static unsigned numberOfSignalBits = numberOfTiles * numberOfPossibleValues;

    static std::vector<double> boardToSignal(const Game2048Core::BoardState &board);
    static std::vector<double> boardToBitSignal(const Game2048Core::BoardState &board);

    static double maxTileValue(const Game2048Core::BoardState &board);
};

}

#endif // BOARDSIGNALCONVERTER_H
