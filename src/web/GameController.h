#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <Wt/WObject.h>
#include <gamecore.h>

namespace nn2048
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
