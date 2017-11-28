#ifndef BASTARDAPPLICATION_H
#define BASTARDAPPLICATION_H

#include <Wt/WApplication.h>
#include <TwoOhFourEightCore/gamecore.h>
#include <TwoOhFourEightCore/gamestatetracker.h>
#include <NeuralNetwork/network.h>

namespace Bastard
{

class GameWidget;
class GameController;

class BastardApplication : public Wt::WApplication
{
public:
    BastardApplication(const Wt::WEnvironment &env, const NeuralNetwork::Network *network = nullptr);

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