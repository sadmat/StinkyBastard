#include "NetworkOutputConverter.h"
#include <stdexcept>
#include <map>
#include <algorithm>

using namespace Game2048Core;

namespace nn2048
{

DirectionSignalVector NetworkOutputConverter::outputToMoves(const std::vector<double> &output)
{
    if (output.size() != static_cast<unsigned>(Direction::Total))
        throw std::runtime_error("Network output has to be the length of possible moves count");
    DirectionSignalVector directions;
    directions.reserve(output.size());
    for (unsigned i = 0; i < output.size(); ++i)
        directions.push_back({ static_cast<Direction>(i), output[i] });
    std::sort(directions.begin(), directions.end(), [] (const DirectionSignal &s1, const DirectionSignal &s2) {
        return s1.second > s2.second;
    });
    return directions;
}

DirectionSignalVector NetworkOutputConverter::outputToMoves(const double *output)
{
    DirectionSignalVector directions;
    directions.reserve(static_cast<size_t>(Direction::Total));
    for (unsigned i = 0; i < static_cast<size_t>(Direction::Total); ++i)
        directions.push_back({ static_cast<Direction>(i), output[i] });
    std::sort(directions.begin(), directions.end(), [] (const DirectionSignal &s1, const DirectionSignal &s2) {
        return s1.second > s2.second;
    });
    return directions;
}

}


