#include "ReplayMemoryTracker.h"
#include "BoardSignalConverter.h"
#include "Reinforcement.h"

namespace nn2048 {

ReplayMemoryTracker::ReplayMemoryTracker(Game2048Core::GameCore *gameCore):
    _gameCore(gameCore)
{
    prepareNewReplayMemory();

    _gameCore->onTryingToMoveTiles.connect([this] (Game2048Core::Direction) {
        _boardSignal = BoardSignalConverter::boardToBitSignal(_gameCore->board());
        _gameScore = _gameCore->score();
    });

    _gameCore->onTilesMoved.connect([this] (Game2048Core::Direction direction, bool succeeded) {
        onTilesMoved(direction, succeeded);
    });

    _gameCore->onReset.connect([this] () {
        onGameReset();
    });
}

void ReplayMemoryTracker::onTilesMoved(Game2048Core::Direction direction, bool succeeded)
{

}

void ReplayMemoryTracker::onGameReset()
{

}

void ReplayMemoryTracker::onGameOver()
{

}

bool ReplayMemoryTracker::serializeReplayMemory(const std::string &fileName)
{
    if (_replayMemory->currentSize() == 0)
        return false;
    return _replayMemory->serialize(fileName);
}

void ReplayMemoryTracker::prepareNewReplayMemory()
{
    _replayMemory = std::make_unique<ReplayMemory>();
}



}
