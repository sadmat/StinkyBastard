#ifndef BOARDSIGNALCONVERTER_H
#define BOARDSIGNALCONVERTER_H

#include <TwoOhFourEightCore/gamecore.h>

namespace Bastard
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
