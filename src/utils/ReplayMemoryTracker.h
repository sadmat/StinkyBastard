#ifndef REPLAYMEMORYTRACKER_H
#define REPLAYMEMORYTRACKER_H

#include <GameCore.h>
#include "ReplayMemory.h"

namespace nn2048 {

/// Keeps track of game flow. Current game states and moves are passed to
/// replay memory and serialized when game is reset or over.
class ReplayMemoryTracker
{
public:
    ReplayMemoryTracker(Game2048Core::GameCore *gameCore);

    bool serializeReplayMemory(const std::string &fileName);
    void prepareNewReplayMemory();

protected:
    void onTilesMoved(Game2048Core::Direction direction, bool succeeded);
    void onGameReset();
    void onGameOver();

private:
    Game2048Core::GameCore *_gameCore;
    std::unique_ptr<ReplayMemory> _replayMemory;
    std::vector<double> _boardSignal;
    unsigned _gameScore;
};

}



#endif // REPLAYMEMORYTRACKER_H
