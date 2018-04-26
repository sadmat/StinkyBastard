#ifndef QLEARNINGSTATE_H
#define QLEARNINGSTATE_H

#include <vector>
#include <GameCore.h>
#include <json/json.h>

namespace nn2048
{

class QLearningState
{
public:
    QLearningState(const std::vector<double> &boardSignal,
                   Game2048Core::Direction takenAction,
                   double receivedReward,
                   bool moveFailed,
                   bool isInTerminalState = false);
    QLearningState(const Json::Value &json);
    QLearningState(const QLearningState &) = delete;
    QLearningState(QLearningState &&other);
    ~QLearningState() = default;

    const std::vector<double> &boardSignal() const { return _boardSignal; }
    Game2048Core::Direction takenAction() const { return _action; }
    double receivedReward() const { return _reward; }
    bool hasMoveFailed() const { return _moveFailed; }
    bool isInTerminalState() const { return _terminalState; }

    void setNextState(const QLearningState * next) { _nextState = next; }
    const QLearningState *nextState() const { return _nextState; }

    QLearningState &operator = (const QLearningState &) = delete;
    QLearningState &operator = (QLearningState &&other);

    Json::Value toJsonValue() const;

protected:
    void deserializeBoardSignal(const Json::Value &json);
    void deserializeTakenAction(const Json::Value &json);
    void deserializeReinforcement(const Json::Value &json);
    void deserializeMoveFailedValue(const Json::Value &json);
    void deserializeTerminalStateValue(const Json::Value &json);

private:
    std::vector<double> _boardSignal;
    Game2048Core::Direction _action;
    double _reward;
    bool _moveFailed;
    bool _terminalState;
    const QLearningState *_nextState = nullptr;
};

}


#endif // QLEARNINGSTATE_H
