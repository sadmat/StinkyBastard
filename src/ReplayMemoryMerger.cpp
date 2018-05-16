#include "ReplayMemoryMerger.h"
#include <iostream>
#include <boost/filesystem.hpp>

namespace nn2048 {

ReplayMemoryMerger::ReplayMemoryMerger(std::unique_ptr<ReplayMemoryMergerArguments> arguments) :
    _arguments(std::move(arguments))
{ }

int ReplayMemoryMerger::run()
{
    auto jsons = scanForJsons();
    if (jsons.empty()) {
        std::clog << "No json files found. Aborting" << std::endl;
        return -1;
    }

    auto replayMemory = loadReplayMemory(jsons);
    if (!replayMemory)
        return -1;
    else {
        std::clog << replayMemory->currentSize() << " game states loaded" << std::endl;
    }

    if (!serializeReplayMemory(*replayMemory))
        return -1;

    return 0;
}

std::vector<std::string> ReplayMemoryMerger::scanForJsons() const
{
    if (!boost::filesystem::is_directory(_arguments->inputDirectory)) {
        std::clog << _arguments->inputDirectory << " is not a directory" << std::endl;
        return {};
    }

    auto fileNames = std::vector<std::string>();
    for (auto &entry: boost::filesystem::directory_iterator(_arguments->inputDirectory)) {
        if (entry.path().extension() == ".json")
            fileNames.push_back(entry.path().string());
    }
    return fileNames;
}

std::unique_ptr<ReplayMemory> ReplayMemoryMerger::loadReplayMemory(const std::vector<std::string> &fileNames) const
{
    auto replayMemory = std::make_unique<ReplayMemory>();
    auto fileCounter = 1;
    for (auto &fileName: fileNames) {
        std::clog << "Loading file " << fileCounter << " of " << fileNames.size() << "...\r" << std::endl;
        try {
            auto gameReplay = std::make_unique<ReplayMemory>(fileName);
            replayMemory->takeStatesFrom(*gameReplay);
        } catch (std::runtime_error &ex) {
            std::clog << std::endl;
            std::clog << "Replay memory loading failed: " << fileName << ", exception: " << ex.what() << std::endl;
            std::clog << "Omitting" << std::endl;
        }
        ++fileCounter;
    }
    std::clog << std::endl;
    return replayMemory;
}

bool ReplayMemoryMerger::serializeReplayMemory(const ReplayMemory &replayMemory)
{
    std::clog << "Serializing replay memory..." << std::endl;
    std::clog.flush();
    if (!replayMemory.serialize(_arguments->outputFileName)) {
        std::clog << "Failed" << std::endl;
        return false;
    } else {
        std::clog << "Finished" << std::endl;
        return true;
    }
}

}
