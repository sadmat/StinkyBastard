#include "ReplayMemory.h"
#include <stdexcept>
#include <set>

namespace nn2048
{

ReplayMemory::ReplayMemory(unsigned size):
    _size(size),
    _distribution(0, _size)
{
    if (_size == 0)
        throw std::invalid_argument("Replay memory size cannot be 0");
}

void ReplayMemory::addState(std::vector<double> boardSignal,
                            Game2048Core::Direction takenAction,
                            double reward,
                            bool isInTerminalState)
{
    if (this->isFull())
        _memory.pop_front();
    auto newState = std::make_unique<QLearningState>(boardSignal,
                                                     takenAction,
                                                     reward,
                                                     isInTerminalState);
    _memory.push_back(std::move(newState));
}

std::vector<const QLearningState *> ReplayMemory::sampleBatch(unsigned size)
{
    if (size > _size)
        throw std::invalid_argument("Sample batch size cannot be greater than replay memory size");
    if (size > _memory.size())
        throw std::invalid_argument("Stample batch size cannot be greater than current replay memory size");

    std::set<unsigned> takenIndices;
    std::vector<const QLearningState *> batch;
    batch.reserve(size);

    while (batch.size() < size)
    {
        unsigned index = _distribution(_randomDevice);
        if (takenIndices.count(index) == 1) continue;
        takenIndices.insert(index);

        auto state = _memory[index].get();
        if (state->isInTerminalState() == false && index < _memory.size() - 1)
            state->setNextState(_memory[index + 1].get());
        else
            state->setNextState(nullptr);
        batch.push_back(state);
    }

    return batch;
}

}
