#ifndef REPLAYMEMORYMERGER_H
#define REPLAYMEMORYMERGER_H

#include "Application.h"
#include <vector>
#include "arguments/ReplayMemoryMergerArguments.h"
#include "utils/ReplayMemory.h"

namespace nn2048 {

class ReplayMemoryMerger : public Application
{
public:
    ReplayMemoryMerger(std::unique_ptr<ReplayMemoryMergerArguments> arguments);

    int run();

protected:
    std::vector<std::string> scanForJsons() const;
    std::unique_ptr<ReplayMemory> loadReplayMemory(const std::vector<std::string> &fileNames) const;
    bool serializeReplayMemory(const ReplayMemory &replayMemory);

private:
    std::unique_ptr<ReplayMemoryMergerArguments> _arguments;
};

}

#endif // REPLAYMEMORYMERGER_H
