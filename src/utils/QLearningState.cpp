#include "QLearningState.h"

namespace nn2048
{

static const std::string BoardSignalKey = "boardSignal";
static const std::string TakenActionKey = "takenAction";
static const std::string ReinforcementKey = "reinforcement";
static const std::string MoveFailedKey = "moveFailed";
static const std::string TerminalStateKey = "terminalState";

std::string directionToString(Game2048Core::Direction direction)
{
    static std::map<Game2048Core::Direction, std::string> dictionary {
        { Game2048Core::Direction::Up, "up" },
        { Game2048Core::Direction::Down, "down" },
        { Game2048Core::Direction::Left, "left" },
        { Game2048Core::Direction::Right, "right" }
    };
    if (dictionary.count(direction) == 0)
        return "";
    return dictionary[direction];
}

Game2048Core::Direction stringToDirection(const std::string &directionString)
{
    static std::map<std::string, Game2048Core::Direction> dictionary {
        { "up", Game2048Core::Direction::Up },
        { "down", Game2048Core::Direction::Down },
        { "left", Game2048Core::Direction::Left },
        { "right", Game2048Core::Direction::Right }
    };
    if (dictionary.count(directionString) == 0)
        return Game2048Core::Direction::None;
    return dictionary[directionString];
}

QLearningState::QLearningState(const std::vector<double> &boardSignal,
                               Game2048Core::Direction takenAction,
                               double receivedReward,
                               bool moveFailed,
                               bool isInTerminalState) :
    _boardSignal(boardSignal),
    _action(takenAction),
    _reward(receivedReward),
    _moveFailed(moveFailed),
    _terminalState(isInTerminalState)
{
}

QLearningState::QLearningState(const Json::Value &json)
{
    deserializeBoardSignal(json);
    deserializeTakenAction(json);
    deserializeReinforcement(json);
    deserializeMoveFailedValue(json);
    deserializeTerminalStateValue(json);
}

QLearningState::QLearningState(QLearningState &&other)
{
    _boardSignal = std::move(other._boardSignal);
    _action = other._action;
    _reward = other._reward;
    _terminalState = other._terminalState;

    other._action = Game2048Core::Direction::None;
    other._reward = 0.0;
    other._terminalState = false;
}

QLearningState &QLearningState::operator =(QLearningState &&other)
{
    _boardSignal = std::move(other._boardSignal);
    _action = other._action;
    _reward = other._reward;
    _terminalState = other._terminalState;

    other._boardSignal.clear();
    other._action = Game2048Core::Direction::None;
    other._reward = 0.0;
    other._terminalState = false;

    return *this;
}

void QLearningState::deserializeBoardSignal(const Json::Value &json)
{
    if (json.isMember(BoardSignalKey)) {
        auto boardSignalJson = json[BoardSignalKey];
        if (boardSignalJson.type() != Json::arrayValue) {
            throw std::runtime_error("boardSignal is not an array");
        }

        _boardSignal.reserve(boardSignalJson.size());
        for (auto &signalJson: boardSignalJson) {
            _boardSignal.push_back(signalJson.asDouble());
        }
    } else {
        throw std::runtime_error("Json does not contain boardSignal array");
    }
}

void QLearningState::deserializeTakenAction(const Json::Value &json)
{
    if (json.isMember(TakenActionKey)) {
        auto takenActionJson = json[TakenActionKey];
        if (takenActionJson.type() != Json::stringValue) {
            throw std::runtime_error("takenAction is not a string");
        }
        _action = stringToDirection(takenActionJson.asString());
    } else {
        throw std::runtime_error("Json does not contain takenAction field");
    }
}

void QLearningState::deserializeReinforcement(const Json::Value &json)
{
    if (json.isMember(ReinforcementKey)) {
        auto reinforcementJson = json[ReinforcementKey];
        if (reinforcementJson.type() != Json::realValue &&
            reinforcementJson.type() != Json::intValue) {
            throw std::runtime_error("reinforcement field is not real nor integer value");
        }
        _reward = reinforcementJson.asDouble();
    } else {
        throw std::runtime_error("Json does not contain reinforcement value");
    }
}

void QLearningState::deserializeMoveFailedValue(const Json::Value &json)
{
    if (json.isMember(MoveFailedKey)) {
        auto moveFailedJson = json[MoveFailedKey];
        if (moveFailedJson.type() != Json::booleanValue) {
            throw std::runtime_error("moveFailed field is not a boolean value");
        }
        _moveFailed = moveFailedJson.asBool();
    } else {
        throw std::runtime_error("Json does not contain moveFailed value");
    }
}

void QLearningState::deserializeTerminalStateValue(const Json::Value &json)
{
    if (json.isMember(TerminalStateKey)) {
        auto terminalStateJson = json[TerminalStateKey];
        if (terminalStateJson.type() != Json::booleanValue) {
            throw std::runtime_error("terminalState field is not a boolean value");
        }
        _terminalState = terminalStateJson.asBool();
    } else {
        throw std::runtime_error("terminalState field not found");
    }
}

Json::Value QLearningState::toJsonValue() const
{
    auto json = Json::Value(Json::objectValue);

    auto boardSignalJson = Json::Value(Json::arrayValue);
    for (auto signal: _boardSignal) {
        boardSignalJson.append(signal);
    }
    json[BoardSignalKey] = boardSignalJson;
    json[TakenActionKey] = directionToString(_action);
    json[ReinforcementKey] = _reward;
    json[MoveFailedKey] = _moveFailed;
    json[TerminalStateKey] = _terminalState;

    return json;
}

}
