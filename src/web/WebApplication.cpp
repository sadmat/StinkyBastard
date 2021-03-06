#include "WebApplication.h"
#include <GameCore.h>
#include <GameHistorySerializer.h>
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
static const std::string RestartParameterName = "restart";
static const std::string AutoRestartValue = "auto";

using namespace Game2048Core;

namespace nn2048
{

WebApplication::WebApplication(const Wt::WEnvironment &env, const NeuralNetwork::Network *network, unsigned long highscoreThreshold):
    Wt::WApplication(env),
    _highscoreThreshold(highscoreThreshold),
    _gameCore(std::make_unique<GameCore>(GAME_BOARD_SIZE)),
    _gameStateTracker(std::make_unique<GameStateTracker>(_gameCore.get())),
    _replayMemoryTracker(std::make_unique<ReplayMemoryTracker>(_gameCore.get()))
{
    setTitle("Bastard - 2048 game played by neural network");
    root()->setContentAlignment(Wt::AlignmentFlag::Center);
    root()->decorationStyle().setBackgroundColor(Wt::WColor(250, 248, 239));

    _gameWidget = root()->addWidget(std::make_unique<GameWidget>());
    _gameWidget->headerWidget()->setBestScore(getBestScoreCookie());

    setupGameController(network);

    _gameCore->onBeingReset.connect([this] () {
        serializeReplayMemory();
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
        serializeReplayMemory();
    });
    _gameCore->onScoreUpdated.connect([this] (unsigned int score) {
        scoreUpdated(score);
    });

    showInitialTiles();
}

void WebApplication::setupGameController(const NeuralNetwork::Network *network)
{
    auto param = environment().getParameter(ControllerParameterName);
    if (param && *param == NeuralNetworkControllerValue && network)
        setupNeuralNetworkGameController(network);
    else setupKeyboardGameController();
}

void WebApplication::setupKeyboardGameController()
{
    KeyboardGameController *controller = this->addChild(std::make_unique<KeyboardGameController>(_gameCore.get(), _gameStateTracker.get()));
    _gameController = controller;
    globalKeyWentDown().connect(controller, &KeyboardGameController::onKeyDown);
}

void WebApplication::setupNeuralNetworkGameController(const NeuralNetwork::Network *network)
{
    bool autoRestart = false;
    auto param = environment().getParameter(RestartParameterName);
    if (param && *param == AutoRestartValue)
        autoRestart = true;
    auto controller = this->addChild(std::make_unique<NeuralNetworkGameController>(_gameCore.get(), network, autoRestart));
    _gameController = controller;
    controller->start();
}

void WebApplication::showInitialTiles() const
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

void WebApplication::serializeReplayMemory() const
{
    if (_gameCore->state().score < _highscoreThreshold)
    {
        _gameStateTracker->reset();
        _replayMemoryTracker->reset();
        return;
    }
    std::string fileName = appRoot();
    Wt::WDateTime dateTime = Wt::WDateTime::currentDateTime();
    fileName += dateTime.toString("yyyy-MM-dd HH.mm.ss.zzz", true).toUTF8();
    fileName += ".json";
    _replayMemoryTracker->serializeReplayMemory(fileName);
    _gameStateTracker->reset();
    _replayMemoryTracker->reset();
}

void WebApplication::scoreUpdated(unsigned int score)
{
    _gameWidget->headerWidget()->setScore(score);
    if (_gameWidget->headerWidget()->bestScore() < score)
    {
        _gameWidget->headerWidget()->setBestScore(score);
        setBestScoreCookie(score);
    }
}

void WebApplication::setBestScoreCookie(unsigned int score)
{
    setCookie(BEST_SCORE_COOKIE, std::to_string(score), 60 * 60 * 24 * 14);
}

unsigned int WebApplication::getBestScoreCookie()
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


