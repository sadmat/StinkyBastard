#ifndef REPLAYMEMORY_H
#define REPLAYMEMORY_H

#include <deque>
#include <memory>
#include "QLearningState.h"

namespace nn2048
{

class ReplayMemory
{
public:
    /// Initializes replay memory without size constraint
    ReplayMemory();

    /// Initializes replay memory with size constraint
    ReplayMemory(unsigned size);

    /// Initializes replay memory from json file
    ReplayMemory(const std::string &fileName);

    /// Serializes replay memory to json
    bool serialize(const std::string &fileName) const;

    void addState(std::vector<double> boardSignal,
                  Game2048Core::Direction takenAction,
                  double reward,
                  bool moveFailed,
                  bool isInTerminalState = false);
    void addState(std::unique_ptr<QLearningState> state);

    std::vector<QLearningState const *> sampleBatch(unsigned size);

    bool isFull() const { return _size > 0 && _memory.size() == _size; }
    unsigned long currentSize() const { return _memory.size(); }

private:
    unsigned _size;
    std::deque<std::unique_ptr<QLearningState>> _memory;
};

}


#endif // REPLAYMEMORY_H
