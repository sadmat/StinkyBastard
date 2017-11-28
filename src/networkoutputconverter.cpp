#include "networkoutputconverter.h"
#include <stdexcept>
#include <map>
#include <algorithm>

using namespace Game2048Core;

namespace Bastard
{

DirectionSignalVector NetworkOutputConverter::outputToMoves(const std::vector<double> &output)
{
    if (output.size() != (unsigned)Direction::Total)
        throw std::runtime_error("Network output has to be the length of possible moves count");
    DirectionSignalVector directions;
    directions.reserve(output.size());
    for (unsigned i = 0; i < output.size(); ++i)
        directions.push_back({ (Direction)i, output[i] });
    std::sort(directions.begin(), directions.end(), [] (const DirectionSignal &s1, const DirectionSignal &s2) {
        return s1.second > s2.second;
    });
    return directions;
}

}


