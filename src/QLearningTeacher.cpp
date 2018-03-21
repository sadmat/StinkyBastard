//
// Created by Mateusz Sądel on 04.03.2018.
//

#include "QLearningTeacher.h"
#include <iostream>
#include <stdexcept>
#include <boost/filesystem.hpp>
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
                                   double momentum)
    :
_networkFileName(networkFileName),
_maxAge(maxAge),
_targetScore(targetScore),
_gamma(gamma),
_learningRate(learningRate),
_momentum(momentum),
_network(nullptr),
_game(std::make_unique<Game2048Core::GameCore>(gameBoardSideLength))
{}

int QLearningTeacher::run()
{
    if (boost::filesystem::exists(_networkFileName))
        _network = loadNeuralNetwork();
    else
    {
        std::cout << "Neural network not found. Creating new one... ";
        _network = createNeuralNetwork();
        std::cout << "ok" << std::endl;
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

std::unique_ptr<FANN::neural_net> QLearningTeacher::createNeuralNetwork() const
{
    const int networkType = FANN::LAYER;
    const unsigned int layerCount = 4;
    const unsigned int neuronCounts[] = { 16, 256, 128, 4 };
    auto network = std::make_unique<FANN::neural_net>(networkType, layerCount, neuronCounts);

    return network;
}

void QLearningTeacher::performLearning() const
{
    ReplayMemory replayMemory(10000);
    unsigned age = 0;
    unsigned agentStepCount = 0;
    unsigned illegalMoves = 0;
    auto shouldContinueLearning = learningCondition(age, _game->state().score);

    _network->set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);
    _network->set_activation_function_output(FANN::LINEAR);
    _network->set_training_algorithm(FANN::TRAIN_BATCH);
    _network->set_learning_rate(static_cast<float>(_learningRate));
    _network->set_learning_momentum(static_cast<float>(_momentum));

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

        // Pick action
        // TODO: random or maxarg
        auto currentStateSignal = BoardSignalConverter::boardToSignal(_game->board());
        double *networkOutput = _network->run(&currentStateSignal[0]);
        auto qValues = NetworkOutputConverter::outputToMoves(networkOutput);
        // TODO: remove output
        unsigned prevScore = _game->score();

        // Carry out action
        bool moveFailed = !_game->tryMove(qValues.front().first);
        double reward = computeReward(moveFailed, prevScore < _game->score());
        auto newStateSignal = BoardSignalConverter::boardToSignal(_game->board());

        // Store replay
        replayMemory.addState(currentStateSignal, qValues.front().first, reward, _game->isGameOver());

        // Get training batch
        unsigned batchSize = 500;
        if (batchSize > replayMemory.currentSize())
            batchSize = static_cast<unsigned>(replayMemory.currentSize());
        auto trainingBatch = replayMemory.sampleBatch(batchSize);

        // Train network
        auto trainingData = prepareTrainingData(trainingBatch);
        _network->train_epoch(trainingData);

        ++age;
    }
}

FANN::training_data QLearningTeacher::prepareTrainingData(const std::vector<const QLearningState *> &batch) const
{
    unsigned learningSetCount = static_cast<unsigned>(batch.size());
    unsigned inputCount = 16;
    unsigned outputCount = 4;
    double **inputSets = new double*[batch.size()];
    double **outputSets = new double*[batch.size()];

    for (unsigned i = 0; i < batch.size(); ++i)
    {
        auto inputs = const_cast<double *>(&(batch[i]->boardSignal()[0]));
        inputSets[i] = inputs;

        auto networkOutputs = _network->run(inputs);
        auto outputs = new double[outputCount];
        for (unsigned j = 0; j < outputCount; ++j)
            outputs[j] = networkOutputs[j];

        double targetValue = batch[i]->receivedReward();
        unsigned targetOutputIndex = static_cast<unsigned>(batch[i]->takenAction());
        if (batch[i]->isInTerminalState() == false && batch[i]->nextState() != nullptr)
        {
            auto nextStateInputs = const_cast<double *>(&(batch[i]->nextState()->boardSignal()[0]));
            auto nextStateOutputs = _network->run(nextStateInputs);
            double nextActionQValue = nextStateOutputs[0];
            for (unsigned j = 1; j < outputCount; ++j)
                if (nextStateOutputs[j] > nextActionQValue)
                    nextActionQValue = nextStateOutputs[j];

            targetValue += _gamma * nextActionQValue;
        }
        outputs[targetOutputIndex] = targetValue;

        outputSets[i] = outputs;
    }

    FANN::training_data trainingData;
    trainingData.set_train_data(learningSetCount,
                                inputCount,
                                inputSets,
                                outputCount,
                                outputSets);
    delete [] inputSets;
    for (unsigned i = 0; i < learningSetCount; ++i)
        delete [] outputSets[i];
    delete [] outputSets;
    return trainingData;
}

void QLearningTeacher::serializeNetwork() const
{
    std::cout << "Serializing network... ";
    _network->save(_networkFileName);
    std::cout << "ok" << std::endl;
}

double QLearningTeacher::computeReward(bool moveFailed, bool scoreIncreased) const
{
    if (moveFailed)
        return -1.0;
    else if (_game->isGameOver())
        return -1.0;
    else if (scoreIncreased)
        return 1.0; // TODO: should reward depend on score?
    else
        return 0.1; // Reward for survival.
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
        auto c= [&score, this] () -> bool {
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
