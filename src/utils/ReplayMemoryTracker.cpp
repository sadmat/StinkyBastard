#include "ReplayMemoryTracker.h"
#include "BoardSignalConverter.h"
#include "Reinforcement.h"

namespace nn2048 {

ReplayMemoryTracker::ReplayMemoryTracker(Game2048Core::GameCore *gameCore):
    _gameCore(gameCore)
{
    reset();

    _gameCore->onTryingToMoveTiles.connect([this] (Game2048Core::Direction) {
        _boardSignal = BoardSignalConverter::boardToBitSignal(_gameCore->board());
        _prevScore = _gameCore->score();
    });

    _gameCore->onTilesMoved.connect([this] (Game2048Core::Direction direction, bool succeeded) {
        onTilesMoved(direction, succeeded);
    });

    _gameCore->onBeingReset.connect([this] () {
        onGameReset();
    });

    _gameCore->onGameOver.connect([this] () {
        onGameOver();
    });
}

void ReplayMemoryTracker::onTilesMoved(Game2048Core::Direction direction, bool succeeded)
{
    saveNewState(succeeded, direction);
}

void ReplayMemoryTracker::onGameReset()
{
    saveNewState(true, Game2048Core::Direction::None);
}

void ReplayMemoryTracker::onGameOver()
{
    saveNewState(true, Game2048Core::Direction::None);
}

bool ReplayMemoryTracker::serializeReplayMemory(const std::string &fileName)
{
    if (_replayMemory->currentSize() == 0)
        return false;
    return _replayMemory->serialize(fileName);
}

void ReplayMemoryTracker::reset()
{
    _replayMemory = std::make_unique<ReplayMemory>();
    _prevScore = 0;
    _boardSignal.clear();
}

void ReplayMemoryTracker::saveNewState(bool succeeded, Game2048Core::Direction direction)
{
    auto reinforcement = Reinforcement::computeReinforcement(_gameCore->isGameOver(), succeeded, _gameCore->score(), _prevScore);
    _replayMemory->addState(_boardSignal, direction, reinforcement, !succeeded, _gameCore->isGameOver());
}


}
