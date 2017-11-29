#ifndef BASTARDAPPLICATION_H
#define BASTARDAPPLICATION_H

#include <Wt/WApplication.h>
#include <GameCore.h>
#include <GameStateTracker.h>
#include <Network.h>

namespace nn2048
{

class GameWidget;
class GameController;

class WebApplication : public Wt::WApplication
{
public:
    WebApplication(const Wt::WEnvironment &env, const NeuralNetwork::Network *network = nullptr);

protected:
    void setupGameController(const NeuralNetwork::Network *network);
    void setupKeyboardGameController();
    void setupNeuralNetworkGameController(const NeuralNetwork::Network *network);

    void showInitialTiles() const;
    void serializeGameHistory() const;
    void scoreUpdated(unsigned int score);

    void setBestScoreCookie(unsigned int score);
    unsigned int getBestScoreCookie();

private:
    std::unique_ptr<Game2048Core::GameCore> _gameCore;
    std::unique_ptr<Game2048Core::GameStateTracker> _gameStateTracker;
    GameWidget *_gameWidget;
    GameController *_gameController;
};

}

#endif // BASTARDAPPLICATION_H
