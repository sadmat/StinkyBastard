#include "WebApplication.h"
#include <gamecore.h>
#include <gamehistoryserializer.h>
#include <Wt/WDateTime.h>
#include <Wt/WEnvironment.h>
#include "GameWidget.h"
#include "KeyboardGameController.h"
#include "NeuralNetworkGameController.h"
#include "GameBoardWidget.h"
#include "GameHeaderWidget.h"
#include "ScoreWidget.h"


#define GAME_BOARD_SIZE 4
#define BEST_SCORE_COOKIE "bestscore"

static const std::string ControllerParameterName = "controller";
static const std::string KeyboardControllerValue = "keyboard";
static const std::string NeuralNetworkControllerValue = "neural";

using namespace Game2048Core;

namespace nn2048
{

BastardApplication::BastardApplication(const Wt::WEnvironment &env, const NeuralNetwork::Network *network):
    Wt::WApplication(env),
    _gameCore(std::make_unique<GameCore>(GAME_BOARD_SIZE)),
    _gameStateTracker(std::make_unique<GameStateTracker>(_gameCore.get()))
{
    setTitle("Bastard - 2048 game played by neural network");
    root()->setContentAlignment(Wt::AlignmentFlag::Center);
    root()->decorationStyle().setBackgroundColor(Wt::WColor(250, 248, 239));

    _gameWidget = root()->addWidget(std::make_unique<GameWidget>());
    _gameWidget->headerWidget()->setBestScore(getBestScoreCookie());

    setupGameController(network);

    _gameCore->onReset.connect([this] () {
        serializeGameHistory();
        _gameWidget->boardWidget()->onReset();
        _gameWidget->headerWidget()->setScore(0);
    });
    _gameCore->onTileMoved.connect([this] (TilePosition from, TilePosition to, bool merged) {
        _gameWidget->boardWidget()->onTileMoved(from, to, merged);
    });
    _gameCore->onTileCreated.connect([this] (unsigned int value, TilePosition position) {
        _gameWidget->boardWidget()->onTileCreated(value, position);
    });
    _gameCore->onGameStateChanged.connect([this] (const GameState &state) {
        _gameWidget->boardWidget()->onGameStateChanged(state);
    });
    _gameCore->onGameOver.connect([this] () {
        serializeGameHistory();
    });
    _gameCore->onScoreUpdated.connect([this] (unsigned int score) {
        scoreUpdated(score);
    });

    showInitialTiles();
}

void BastardApplication::setupGameController(const NeuralNetwork::Network *network)
{
    auto param = environment().getParameter(ControllerParameterName);
    if (param && *param == NeuralNetworkControllerValue && network)
        setupNeuralNetworkGameController(network);
    else setupKeyboardGameController();
}

void BastardApplication::setupKeyboardGameController()
{
    KeyboardGameController *controller = this->addChild(std::make_unique<KeyboardGameController>(_gameCore.get(), _gameStateTracker.get()));
    _gameController = controller;
    globalKeyWentDown().connect(controller, &KeyboardGameController::onKeyDown);
}

void BastardApplication::setupNeuralNetworkGameController(const NeuralNetwork::Network *network)
{
    auto controller = this->addChild(std::make_unique<NeuralNetworkGameController>(_gameCore.get(), network));
    _gameController = controller;
    controller->start();
}

void BastardApplication::showInitialTiles() const
{
    std::vector<TilePosition> positions;
    for (unsigned int i = 0; i < _gameCore->board().size() && positions.size() < 2; ++i)
    {
        for (unsigned int j = 0; j < _gameCore->board()[i].size() && positions.size() < 2; ++j)
        {
            const Tile &tile = _gameCore->board()[i][j];
            if (!tile.isEmpty())
                positions.push_back({i, j});
        }
    }
    _gameWidget->boardWidget()->setInitialTiles(positions[0], positions[1]);
}

void BastardApplication::serializeGameHistory() const
{
    std::string fileName = appRoot();
    Wt::WDateTime dateTime = Wt::WDateTime::currentDateTime();
    fileName += dateTime.toString("yyyy-MM-dd HH.mm.ss.zzz", true).toUTF8();
    fileName += ".json";
    GameHistorySerializer::serialize(_gameStateTracker->moves(), fileName);
    _gameStateTracker->reset();
}

void BastardApplication::scoreUpdated(unsigned int score)
{
    _gameWidget->headerWidget()->setScore(score);
    if (_gameWidget->headerWidget()->bestScore() < score)
    {
        _gameWidget->headerWidget()->setBestScore(score);
        setBestScoreCookie(score);
    }
}

void BastardApplication::setBestScoreCookie(unsigned int score)
{
    setCookie(BEST_SCORE_COOKIE, std::to_string(score), 60 * 60 * 24 * 14);
}

unsigned int BastardApplication::getBestScoreCookie()
{
    const std::string *cookie = environment().getCookie(BEST_SCORE_COOKIE);
    unsigned int score = 0;
    if (cookie)
    {
        try
        {
            score = std::stoi(*cookie);
        }
        catch (std::exception exception)
        {
            std::cerr << "BastardApplication::getBestScoreCookie() - exception caught: " << exception.what() << std::endl;
        }
    }
    return score;
}

}


