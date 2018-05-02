#ifndef BASTARDAPPLICATION_H
#define BASTARDAPPLICATION_H

#include <Wt/WApplication.h>
#include <GameCore.h>
#include <GameStateTracker.h>
#include <Network.h>
#include "../utils/Defaults.h"
#include "../utils/ReplayMemoryTracker.h"

namespace nn2048
{

class GameWidget;
class GameController;

class WebApplication : public Wt::WApplication
{
public:
    WebApplication(const Wt::WEnvironment &env,
                   const NeuralNetwork::Network *network = nullptr,
                   unsigned long highscoreThreshold = DefaultHighscoreToRecordThreshold);

protected:
    void setupGameController(const NeuralNetwork::Network *network);
    void setupKeyboardGameController();
    void setupNeuralNetworkGameController(const NeuralNetwork::Network *network);

    void showInitialTiles() const;
    void serializeReplayMemory() const;
    void scoreUpdated(unsigned int score);

    void setBestScoreCookie(unsigned int score);
    unsigned int getBestScoreCookie();

private:
    unsigned long _highscoreThreshold;
    std::unique_ptr<Game2048Core::GameCore> _gameCore;
    std::unique_ptr<Game2048Core::GameStateTracker> _gameStateTracker;
    std::unique_ptr<ReplayMemoryTracker> _replayMemoryTracker;
    GameWidget *_gameWidget;
    GameController *_gameController;
};

}

#endif // BASTARDAPPLICATION_H
