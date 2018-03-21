#ifndef NETWORKOUTPUTCONVERTER_H
#define NETWORKOUTPUTCONVERTER_H

#include <array>
#include <GameCore.h>

namespace nn2048
{

typedef std::pair<Game2048Core::Direction, double> DirectionSignal;
typedef std::vector<DirectionSignal> DirectionSignalVector;

class NetworkOutputConverter
{
public:
    static DirectionSignalVector outputToMoves(const std::vector<double> &output);
    static DirectionSignalVector outputToMoves(const double *output);
};

}

#endif // NETWORKOUTPUTCONVERTER_H
