#ifndef TILEPOSITIONCOMPARER_H
#define TILEPOSITIONCOMPARER_H

#include <tile.h>

namespace Bastard
{

using namespace Game2048Core;

class TilePositionComparer
{
public:
    bool lesser(const TilePosition &pos1, const TilePosition &pos2) const;

    bool operator()(const TilePosition &pos1, const TilePosition &pos2) const { return lesser(pos1, pos2); }
};

}

#endif // TILEPOSITIONCOMPARER_H
