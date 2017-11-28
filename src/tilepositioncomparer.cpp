#include "tilepositioncomparer.h"

namespace Bastard
{

bool TilePositionComparer::lesser(const TilePosition &pos1, const TilePosition &pos2) const
{
    if (pos1.column < pos2.column)
        return true;
    else if (pos2.column < pos1.column)
        return false;
    return pos1.row < pos2.row;
}

}


