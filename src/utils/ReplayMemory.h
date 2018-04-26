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
    ReplayMemory(unsigned size);
    ReplayMemory(const std::string &fileName);

    bool serialize(const std::string &fileName) const;

    void addState(std::vector<double> boardSignal,
                  Game2048Core::Direction takenAction,
                  double reward,
                  bool moveFailed,
                  bool isInTerminalState = false);
    void addState(std::unique_ptr<QLearningState> state);

    std::vector<QLearningState const *> sampleBatch(unsigned size);

    bool isFull() const { return _memory.size() == _size; }
    unsigned long currentSize() const { return _memory.size(); }

private:
    unsigned _size;
    std::deque<std::unique_ptr<QLearningState>> _memory;
};

}


#endif // REPLAYMEMORY_H
