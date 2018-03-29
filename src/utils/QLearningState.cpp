#include "QLearningState.h"

namespace nn2048
{

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

}
