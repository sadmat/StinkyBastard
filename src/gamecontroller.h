#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <Wt/WObject.h>
#include <TwoOhFourEightCore/gamecore.h>

namespace Bastard
{

using namespace Game2048Core;

class GameController : public Wt::WObject
{
public:
    GameController(GameCore *gameCore);

protected:
    GameCore *_gameCore;
};

}


#endif // GAMECONTROLLER_H
