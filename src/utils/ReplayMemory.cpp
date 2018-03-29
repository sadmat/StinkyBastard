#include "ReplayMemory.h"
#include <stdexcept>
#include <set>
#include <random>
#include <cstdlib>

namespace nn2048
{

ReplayMemory::ReplayMemory(unsigned size):
    _size(size)
{
    if (_size == 0)
        throw std::invalid_argument("Replay memory size cannot be 0");

    std::random_device randomDevice;
    std::uniform_int_distribution<unsigned> distribution(0, static_cast<unsigned>(time(nullptr)));
    srand(distribution(randomDevice));
}

void ReplayMemory::addState(std::vector<double> boardSignal,
                            Game2048Core::Direction takenAction,
                            double reward,
                            bool moveFailed,
                            bool isInTerminalState)
{
    if (this->isFull())
        _memory.pop_front();
    auto newState = std::make_unique<QLearningState>(boardSignal,
                                                     takenAction,
                                                     reward,
                                                     moveFailed,
                                                     isInTerminalState);
    _memory.push_back(std::move(newState));
}

std::vector<const QLearningState *> ReplayMemory::sampleBatch(unsigned size)
{
    if (!size)
        throw std::invalid_argument("Sample batch size cannot be 0");
    if (size > _size)
        throw std::invalid_argument("Sample batch size cannot be greater than replay memory size");
    if (size > _memory.size())
        throw std::invalid_argument("Stample batch size cannot be greater than current replay memory size");

    std::set<unsigned> takenIndices;
    std::vector<const QLearningState *> batch;
    batch.reserve(size);

    while (batch.size() < size)
    {
        unsigned index = static_cast<unsigned>(rand()) % _memory.size();
        if (takenIndices.count(index) == 1) continue;
        takenIndices.insert(index);

        auto state = _memory[index].get();
        if (state->isInTerminalState() == false && index < _memory.size() - 1)
            state->setNextState(_memory[index + 1].get());
        else if (state->hasMoveFailed())
            state->setNextState(state);
        else
            state->setNextState(nullptr);
        batch.push_back(state);
    }

    return batch;
}

}
