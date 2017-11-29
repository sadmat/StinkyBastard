#ifndef BOARDSIGNALCONVERTER_H
#define BOARDSIGNALCONVERTER_H

#include <GameCore.h>

namespace nn2048
{

class BoardSignalConverter
{
public:
    static std::vector<double> boardToSignal(const Game2048Core::BoardState &board);

protected:
    static double maxTileValue(const Game2048Core::BoardState &board);
};

}

#endif // BOARDSIGNALCONVERTER_H
