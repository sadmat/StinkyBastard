//
// Created by Mateusz Sądel on 04.03.2018.
//

#include "QLearningTeacher.h"
#include <iostream>
#include <stdexcept>
#include <boost/filesystem.hpp>
#include <random>
#include "utils/BoardSignalConverter.h"
#include "utils/NetworkOutputConverter.h"
#include "utils/ReplayMemory.h"
#include "utils/Reinforcement.h"

namespace nn2048
{

const unsigned gameBoardSideLength = 4;

QLearningTeacher::QLearningTeacher(std::unique_ptr<QLearningArguments> arguments) :
      _arguments(std::move(arguments)),
      _network(nullptr),
      _game(std::make_unique<Game2048Core::GameCore>(gameBoardSideLength))
{}

int QLearningTeacher::run()
{
    if (boost::filesystem::exists(_arguments->networkFileName))
        _network = loadNeuralNetwork();
    else
    {
        std::cout << "Neural network not found.";
        return -1;
    }
    if (!_network)
        return -1;
    std::cout << "Learning starts..." << std::endl;
    performLearning();
    serializeNetwork();
    return 0;
}

void QLearningTeacher::onSigInt()
{
    _sigIntCaught = true;
}

std::unique_ptr<FANN::neural_net> QLearningTeacher::loadNeuralNetwork() const
{
    try
    {
        std::cout << "Loading neural network... ";
        std::cout.flush();

        auto network = std::make_unique<FANN::neural_net>(_arguments->networkFileName);

        std::cout << "ok" << std::endl;
        return network;
    }
    catch (std::runtime_error &exception)
    {
        std::cout << "failed" << std::endl;
        std::cerr << "Runtime exception: " << exception.what() << std::endl;
    }
    return nullptr;
}

void QLearningTeacher::performLearning() const
{
    ReplayMemory replayMemory(_arguments->replayMemorySize);
    unsigned age = 0;
    unsigned agentStepCount = 0;
    unsigned illegalMoves = 0;
    bool prevMoveFailed = false;
    double lossSum = 0;
    double currentLossSum = 0;
    Game2048Core::Direction prevDirection = Game2048Core::Direction::None;
    auto shouldContinueLearning = learningCondition(age, _game->state().score);

    _network->set_learning_rate(static_cast<float>(_arguments->learningRate));
    _network->set_learning_momentum(static_cast<float>(_arguments->momentumFactor));

    std::default_random_engine randomEngine;
    std::uniform_real_distribution<double> randomDistrib(0, 1);

    while (shouldContinueLearning() && !_sigIntCaught)
    {
        if (_game->isGameOver())
        {
            printStats(age, _game->score(), agentStepCount, illegalMoves, lossSum / age, currentLossSum / agentStepCount);
            _game->reset();
            agentStepCount = 0;
            currentLossSum = 0;
            illegalMoves = 0;
        }
        else if (agentStepCount > 0 && agentStepCount % 1000 == 0)
            printStats(age, _game->score(), agentStepCount, illegalMoves, lossSum / age, currentLossSum / agentStepCount);

        auto currentStateSignal = BoardSignalConverter::boardToBitSignal(_game->board());

        // Pick action
        Game2048Core::Direction pickedDirection;
        if (randomDistrib(randomEngine) <= _arguments->epsilonFactor) {
            // Random
            int totalDirections = static_cast<unsigned>(Game2048Core::Direction::Total);
            pickedDirection = static_cast<Game2048Core::Direction>(rand() % totalDirections);
        }
        else {
            // Best
            double *networkOutput = _network->run(&currentStateSignal[0]);
            auto qValues = NetworkOutputConverter::outputToMoves(networkOutput);
            pickedDirection = qValues.front().first;
        }

        unsigned prevScore = _game->score();

        // Carry out action
        bool moveFailed = !_game->tryMove(pickedDirection);
        double reward = Reinforcement::computeReinforcement(_game.get(), !moveFailed, _game->score() - prevScore);
        auto newStateSignal = BoardSignalConverter::boardToBitSignal(_game->board());

        // Store replay
        if (!moveFailed || prevDirection != pickedDirection || !prevMoveFailed)
            replayMemory.addState(currentStateSignal, pickedDirection, reward, moveFailed, _game->isGameOver());

        // Get training batch
        unsigned batchSize = _arguments->replayBatchSize;
        if (batchSize > replayMemory.currentSize())
            batchSize = static_cast<unsigned>(replayMemory.currentSize());
        auto trainingBatch = replayMemory.sampleBatch(batchSize);

        auto loss = trainNetwork(trainingBatch);
        lossSum += loss;
        currentLossSum += loss;

        ++age;
        ++agentStepCount;
        if (moveFailed)
            ++illegalMoves;
        prevMoveFailed = moveFailed;
        prevDirection = pickedDirection;
    }
    std::cout << "Learning finished with stats: " << std::endl;
    printStats(age, _game->score(), agentStepCount, illegalMoves, lossSum / age, currentLossSum / agentStepCount);
}

double QLearningTeacher::trainNetwork(const std::vector<const QLearningState *> &batch) const
{
    const unsigned outputCount = static_cast<unsigned>(Game2048Core::Direction::Total);
    double outputs[outputCount];
    double lossSum = 0.0;
    for (auto &state: batch) {
        double *inputs = const_cast<double *>(&(state->boardSignal()[0]));
        auto response = _network->run(inputs);
        for (unsigned i = 0; i < outputCount; ++i)
            outputs[i] = response[i];

        double targetValue = state->receivedReward();
        unsigned targetOutputIndex = static_cast<unsigned>(state->takenAction());
        if (state->hasMoveFailed()) {
            targetValue += _arguments->gamma * outputs[targetOutputIndex];
        } else if (state->isInTerminalState() == false && state->nextState() != nullptr) {
            auto nextStateInputs = const_cast<double *>(&(state->nextState()->boardSignal()[0]));
            auto nextStateOutputs = _network->run(nextStateInputs);
            double nextActionQValue = nextStateOutputs[0];
            for (unsigned j = 1; j < outputCount; ++j)
                if (nextStateOutputs[j] > nextActionQValue)
                    nextActionQValue = nextStateOutputs[j];

            targetValue += _arguments->gamma * nextActionQValue;
        }
        double loss = (targetValue - outputs[targetOutputIndex]);
        loss *= loss;
        loss *= 0.5;
        lossSum += loss;
        outputs[targetOutputIndex] = targetValue;

        _network->train(inputs, outputs);
    }
    return lossSum / static_cast<double>(batch.size());
}

void QLearningTeacher::serializeNetwork() const
{
    std::cout << "Serializing network... ";
    _network->save(_arguments->networkFileName);
    std::cout << "ok" << std::endl;
}

std::function<bool()> QLearningTeacher::learningCondition(const unsigned &age, const unsigned &score) const
{
    if (_arguments->maxAge > 0 && _arguments->targetScore > 0)
    {
        auto c = [&age, &score, this] () -> bool {
            return age < this->_arguments->maxAge && score < this->_arguments->targetScore;
        };
        return std::bind(c);
    }
    else if (_arguments->maxAge > 0)
    {
        auto c = [&age, this] () -> bool {
            return age < this->_arguments->maxAge;
        };
        return std::bind(c);
    }
    else if (_arguments->targetScore > 0)
    {
        auto c = [&score, this] () -> bool {
            return score < _arguments->targetScore;
        };
        return std::bind(c);
    }
    return std::bind([] () -> bool { return false; });
}

void QLearningTeacher::printStats(unsigned epoch, unsigned score, unsigned steps, unsigned illegalSteps, double loss, double currentLoss) const
{
    std::cout << "[age:\t" << epoch
              << "] score:\t" << score
              << ", steps:\t" << steps
              << ", illegal steps:\t" << illegalSteps
              << " (" << (double(illegalSteps)/steps) * 100
              << "%) loss:\t " << loss
              << ", current: \t" << currentLoss << std::endl;
}

}
