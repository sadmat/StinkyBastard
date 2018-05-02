#include "ReplayMemory.h"
#include <stdexcept>
#include <set>
#include <random>
#include <cstdlib>
#include <fstream>

namespace nn2048
{

static const std::string SizeKey = "memorySize";
static const std::string StatesKey = "states";

ReplayMemory::ReplayMemory()
{
    ReplayMemory(0);
}

ReplayMemory::ReplayMemory(unsigned size):
    _size(size)
{
    std::random_device randomDevice;
    std::uniform_int_distribution<unsigned> distribution(0, static_cast<unsigned>(time(nullptr)));
    srand(distribution(randomDevice));
}

ReplayMemory::ReplayMemory(const std::string &fileName)
{
    std::ifstream file(fileName);
    if (!file)
        throw std::runtime_error("Deserialization failed. Cannot open file " + fileName);
    auto json = Json::Value();
    file >> json;
    file.close();

    if (json.isMember(SizeKey)) {
        auto sizeJson = json[SizeKey];
        if (sizeJson.type() != Json::intValue) {
            throw std::runtime_error("Replay memory size field has to be an integer");
        }
        auto size = sizeJson.asInt();
        if (size <= 0) {
            throw std::runtime_error("Replay memory size field cannot be lesser or equal to 0");
        }
        ReplayMemory(static_cast<unsigned>(size));
    } else {
        throw std::runtime_error("Missing replay memory size field");
    }

    if (json.isMember(StatesKey)) {
        auto statesJson = json[StatesKey];
        if (statesJson.type() != Json::arrayValue) {
            throw std::runtime_error("Replay memory states field has to be an array");
        }
        for (auto stateJson: statesJson) {
            auto state = std::make_unique<QLearningState>(stateJson);
            addState(std::move(state));
        }
    } else {
        throw std::runtime_error("Missing replay memory states array");
    }
}

bool ReplayMemory::serialize(const std::string &fileName) const
{
    auto json = Json::Value(Json::objectValue);
    json[SizeKey] = _size > 0 ? _size : static_cast<unsigned>(_memory.size());

    auto statesArray = Json::Value(Json::arrayValue);
    for (const auto &state: _memory) {
        statesArray.append(state->toJsonValue());
    }
    json[StatesKey] = statesArray;

    std::ofstream file(fileName);
    if (!file)
        return false;

    Json::StreamWriterBuilder builder;
    builder.settings_["indentation"] = "";
    auto writer = builder.newStreamWriter();
    writer->write(json, &file);
    file.close();
    return true;
}

void ReplayMemory::addState(std::vector<double> boardSignal,
                            Game2048Core::Direction takenAction,
                            double reward,
                            bool moveFailed,
                            bool isInTerminalState)
{
    auto newState = std::make_unique<QLearningState>(boardSignal,
                                                     takenAction,
                                                     reward,
                                                     moveFailed,
                                                     isInTerminalState);
    addState(std::move(newState));
}

void ReplayMemory::addState(std::unique_ptr<QLearningState> state)
{
    if (this->isFull())
        _memory.pop_front();
    _memory.push_back(std::move(state));
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
