#ifndef NETWORKOUTPUTCONVERTER_H
#define NETWORKOUTPUTCONVERTER_H

#include <array>
#include <gamecore.h>

namespace Bastard
{

typedef std::pair<Game2048Core::Direction, double> DirectionSignal;
typedef std::vector<DirectionSignal> DirectionSignalVector;

class NetworkOutputConverter
{
public:
    static DirectionSignalVector outputToMoves(const std::vector<double> &output);
};

}

#endif // NETWORKOUTPUTCONVERTER_H
