#include "KeyboardGameController.h"
#include <Wt/WEvent.h>
#include <iostream>

namespace nn2048
{

std::map<Wt::Key, Direction> KeyboardGameController::_translationMap = std::map<Wt::Key, Direction>();

KeyboardGameController::KeyboardGameController(GameCore *gameCore, GameStateTracker *stateTracker):
    GameController(gameCore),
    _stateTracker(stateTracker)
{
    initializeTranslationMap();
}

void KeyboardGameController::onKeyDown(const Wt::WKeyEvent &event)
{
    Direction dir = direction(event.key());
    if (dir != Direction::None)
    {
        if (!_gameCore->tryMove(dir))
            std::cout << "KeyboardGameController::onKeyDown: couldn't move tiles" << std::endl;
    }
    else if (event.key() == Wt::Key::R)
        _gameCore->reset();
    else if (event.key() == Wt::Key::U)
    {
        if (_stateTracker)
            _stateTracker->undoLastMove();
        else std::cout << "KeyboardGameController::onKeyDown: no state tracker to undo move" << std::endl;
    }
    else std::cout << "KeyboardGameController::onKeyDown: unknown input" << std::endl;
}

void KeyboardGameController::initializeTranslationMap()
{
    if (_translationMap.size())
        return;
    _translationMap[Wt::Key::Up] = Direction::Up;
    _translationMap[Wt::Key::Down] = Direction::Down;
    _translationMap[Wt::Key::Left] = Direction::Left;
    _translationMap[Wt::Key::Right] = Direction::Right;
}

Direction KeyboardGameController::direction(Wt::Key key)
{
    if (_translationMap.count(key) > 0)
        return _translationMap[key];
    return Direction::None;
}

}


