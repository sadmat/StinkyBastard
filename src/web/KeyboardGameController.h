#ifndef KEYBOARDGAMECONTROLLER_H
#define KEYBOARDGAMECONTROLLER_H

#include "GameController.h"
#include <map>
#include <gamestatetracker.h>

namespace nn2048
{

class KeyboardGameController : public GameController
{
public:
    KeyboardGameController(GameCore *gameCore, GameStateTracker *stateTracker);

    void onKeyDown(const Wt::WKeyEvent &event);

protected:
    static void initializeTranslationMap();
    static Direction direction(Wt::Key key);
    static std::map<Wt::Key, Direction> _translationMap;

    GameStateTracker *_stateTracker;
};

}

#endif // KEYBOARDGAMECONTROLLER_H
