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

namespace nn2048
{

const unsigned gameBoardSideLength = 4;

QLearningTeacher::QLearningTeacher(const std::string &networkFileName,
                                   unsigned maxAge,
                                   unsigned targetScore,
                                   double gamma,
                                   double learningRate,
                                   double momentum,
                                   double epsilon)
    :
      _networkFileName(networkFileName),
      _maxAge(maxAge),
      _targetScore(targetScore),
      _gamma(gamma),
      _learningRate(learningRate),
      _momentum(momentum),
      _epsilon(epsilon),
      _network(nullptr),
      _game(std::make_unique<Game2048Core::GameCore>(gameBoardSideLength))
{}

int QLearningTeacher::run()
{
    if (boost::filesystem::exists(_networkFileName))
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

        auto network = std::make_unique<FANN::neural_net>(_networkFileName);

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
    ReplayMemory replayMemory(20000);
    unsigned age = 0;
    unsigned agentStepCount = 0;
    unsigned illegalMoves = 0;
    bool prevMoveFailed = false;
    Game2048Core::Direction prevDirection = Game2048Core::Direction::None;
    auto shouldContinueLearning = learningCondition(age, _game->state().score);

    _network->set_learning_rate(static_cast<float>(_learningRate));
    _network->set_learning_momentum(static_cast<float>(_momentum));

    std::default_random_engine randomEngine;
    std::uniform_real_distribution<double> randomDistrib(0, 1);

    while (shouldContinueLearning() && !_sigIntCaught)
    {
        if (_game->isGameOver())
        {
            printStats(age, _game->score(), agentStepCount, illegalMoves);
            _game->reset();
            agentStepCount = 0;
            illegalMoves = 0;
        }
        else if (agentStepCount > 0 && agentStepCount % 1000 == 0)
            printStats(age, _game->score(), agentStepCount, illegalMoves);

        auto currentStateSignal = BoardSignalConverter::boardToSignal(_game->board());

        // Pick action
        Game2048Core::Direction pickedDirection;
        if (randomDistrib(randomEngine) <= _epsilon) {
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
        double reward = computeReward(moveFailed, _game->score() - prevScore);
        auto newStateSignal = BoardSignalConverter::boardToSignal(_game->board());

        // Store replay
        if (!moveFailed || prevDirection != pickedDirection || !prevMoveFailed)
            replayMemory.addState(currentStateSignal, pickedDirection, reward, _game->isGameOver());

        // Get training batch
        unsigned batchSize = 1000;
        if (batchSize > replayMemory.currentSize())
            batchSize = static_cast<unsigned>(replayMemory.currentSize());
        auto trainingBatch = replayMemory.sampleBatch(batchSize);

        trainNetwork(trainingBatch);

        ++age;
        ++agentStepCount;
        if (moveFailed)
            ++illegalMoves;
        prevMoveFailed = moveFailed;
        prevDirection = pickedDirection;
    }
    std::cout << "Learning finished with stats: " << std::endl;
    printStats(age, _game->score(), agentStepCount, illegalMoves);
}

void QLearningTeacher::trainNetwork(const std::vector<const QLearningState *> &batch) const
{
    const unsigned outputCount = static_cast<unsigned>(Game2048Core::Direction::Total);
    double outputs[outputCount];
    for (auto &state: batch) {
        double *inputs = const_cast<double *>(&(state->boardSignal()[0]));
        auto response = _network->run(inputs);
        for (unsigned i = 0; i < outputCount; ++i)
            outputs[i] = response[i];

        double targetValue = state->receivedReward();
        unsigned targetOutputIndex = static_cast<unsigned>(state->takenAction());
        if (state->isInTerminalState() == false && state->nextState() != nullptr)
        {
            auto nextStateInputs = const_cast<double *>(&(state->nextState()->boardSignal()[0]));
            auto nextStateOutputs = _network->run(nextStateInputs);
            double nextActionQValue = nextStateOutputs[0];
            for (unsigned j = 1; j < outputCount; ++j)
                if (nextStateOutputs[j] > nextActionQValue)
                    nextActionQValue = nextStateOutputs[j];

            targetValue += _gamma * nextActionQValue;
        }
        outputs[targetOutputIndex] = targetValue;

        _network->train(inputs, outputs);
    }
}

void QLearningTeacher::serializeNetwork() const
{
    std::cout << "Serializing network... ";
    _network->save(_networkFileName);
    std::cout << "ok" << std::endl;
}

double QLearningTeacher::computeReward(bool moveFailed, unsigned deltaScore) const
{
    if (moveFailed)
        return -1.0;
    else if (_game->isGameOver())
        return -1.0;
    else if (deltaScore > 0)
    {
        auto maxTileValue = BoardSignalConverter::maxTileValue(_game->board());
        return (static_cast<double>(deltaScore) / maxTileValue) * 2.0;
    }
    return 0.0;
}

std::function<bool()> QLearningTeacher::learningCondition(const unsigned &age, const unsigned &score) const
{
    if (_maxAge > 0 && _targetScore > 0)
    {
        auto c = [&age, &score, this] () -> bool {
            return age < this->_maxAge && score < this->_targetScore;
        };
        return std::bind(c);
    }
    else if (_maxAge > 0)
    {
        auto c = [&age, this] () -> bool {
            return age < this->_maxAge;
        };
        return std::bind(c);
    }
    else if (_targetScore > 0)
    {
        auto c = [&score, this] () -> bool {
            return score < _targetScore;
        };
        return std::bind(c);
    }
    return std::bind([] () -> bool { return false; });
}

void QLearningTeacher::printStats(unsigned epoch, unsigned score, unsigned steps, unsigned illegalSteps) const
{
    std::cout << "[age:\t" << epoch
              << "] score:\t" << score
              << ", steps:\t" << steps
              << ", illegal steps:\t" << illegalSteps
              << " (" << (double(illegalSteps)/steps) * 100 << "%)" << std::endl;
}

}
