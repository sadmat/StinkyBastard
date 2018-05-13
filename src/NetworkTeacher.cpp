#include "NetworkTeacher.h"
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <stdexcept>
#include "utils/BoardSignalConverter.h"

namespace nn2048
{

NetworkTeacher::NetworkTeacher(std::unique_ptr<NetworkTeacherArguments> arguments):
    _arguments(std::move(arguments)),
    _sigIntCaught(false)
{}

int NetworkTeacher::run()
{
    std::clog << "Initializing..." << std::endl;
    if (!initialize()) {
        std::clog << "Initialization failed. Aborting." << std::endl;
        return -1;
    }

    std::clog << "Training starts..." << std::endl;
    std::clog.flush();
    performTraining();

    std::clog << "Training finished. Serializing network... ";
    std::clog.flush();
    if (!serializeNetwork()) {
        std::clog << "failed" << std::endl;
        return -1;
    }
    std::clog << "ok" << std::endl;
    return 0;
}

void NetworkTeacher::onSigInt()
{
    std::clog << "SIGINT caught. Aborting..." << std::endl;
    std::clog.flush();
    _sigIntCaught = true;
}

bool NetworkTeacher::initialize()
{
    std::clog << "Loading neural network..." << std::endl;
    std::clog.flush();
    _network = loadNeuralNetwork();
    if (!_network) {
        return false;
    }
    std::clog << "Neural network loaded" << std::endl;

    std::clog << "Loading replay memory..." << std::endl;
    std::clog.flush();
    _replayMemory = loadReplayMemory();
    if (!_replayMemory) {
        return false;
    } else if (_replayMemory->currentSize() == 0) {
        std::clog << "Loaded replay memory is empty" << std::endl;
        return false;
    }
    std::clog << "Replay memory loaded" << std::endl;
    return true;
}

std::unique_ptr<FANN::neural_net> NetworkTeacher::loadNeuralNetwork()
{
    try {
        auto network = std::make_unique<FANN::neural_net>(_arguments->networkFileName);
        if (network->get_num_input() != BoardSignalConverter::numberOfSignalBits) {
            std::clog << "Neural network has incompatible number of input neurons: " << network->get_num_input() << std::endl;
            std::clog << "Expected: " << BoardSignalConverter::numberOfSignalBits << std::endl;
            return nullptr;
        } else if (network->get_num_output() != 4) {
            std::clog << "Neural network has incompatible number of output neurons: " << network->get_num_output() << std::endl;
            std::clog << "Expected: 4" << std::endl;
            return nullptr;
        }
        return network;
    } catch (...) {
        std::clog << "Unknown exception caught" << std::endl;
        return nullptr;
    }
}

std::unique_ptr<ReplayMemory> NetworkTeacher::loadReplayMemory()
{
    auto fileNames = replayMemoryFileNames();
    if (fileNames.empty())
        return nullptr;

    auto replayMemory = std::make_unique<ReplayMemory>();
    for (auto &fileName: fileNames) {
        try {
            auto gameReplay = std::make_unique<ReplayMemory>(fileName);
            computeQValues(*gameReplay);
            replayMemory->takeStatesFrom(*gameReplay);
        } catch (std::runtime_error &ex) {
            std::clog << "Replay memory loading failed: " << fileName << ", exception: " << ex.what() << std::endl;
            std::clog << "Omitting" << std::endl;
        }
    }
    return replayMemory;
}

std::vector<std::string> NetworkTeacher::replayMemoryFileNames()
{
    if (!boost::filesystem::is_directory(_arguments->replayMemoryDirectory)) {
        return {};
    }

    auto fileNames = std::vector<std::string>();
    for (auto &entry: boost::filesystem::directory_iterator(_arguments->replayMemoryDirectory)) {
        if (entry.path().extension() == ".json")
            fileNames.push_back(entry.path().string());
    }
    return fileNames;
}

void NetworkTeacher::computeQValues(const ReplayMemory &replayMemory)
{
    double prevQValue = 0.0;
    for (auto it = replayMemory.states().rbegin(); it != replayMemory.states().rend(); ++it) {
        auto &state = *it;
        double qvalue = state->receivedReward() + _arguments->gamma * prevQValue;
        prevQValue = qvalue;
        _qvalueCache[state.get()] = qvalue;
    }
}

void NetworkTeacher::performTraining()
{
    _network->set_learning_rate(static_cast<float>(_arguments->learningRate));
    _network->set_learning_momentum(static_cast<float>(_arguments->momentum));

    for (unsigned epoch = 1; epoch <= _arguments->maxEpochs && !_sigIntCaught; ++epoch) {
        auto trainingBatch = _replayMemory->sampleBatch(static_cast<unsigned int>(_replayMemory->currentSize()));
        unsigned age = 0;
        double totalLoss = 0.0;

        for (auto &state: trainingBatch) {
            if (_sigIntCaught)
                break;

            totalLoss += trainNetwork(state);
            ++age;
            if (age % 1000 == 0)
                printStats(totalLoss, epoch, age);
        }

        if (age % 1000 != 0)
            printStats(totalLoss, epoch, age);
    }
}

double NetworkTeacher::trainNetwork(const QLearningState *state)
{
    double loss = 0.0;
    auto inputs = const_cast<double *>(&(state->boardSignal()[0]));
    double outputs[4];
    double targetValue = _qvalueCache.at(state);

    if (state->isInTerminalState() || state->takenAction() == Game2048Core::Direction::None) {
        for (unsigned long i = 0; i < sizeof(outputs) / sizeof(outputs[0]); ++i)
            outputs[i] = targetValue;
    } else {
        auto response = _network->run(inputs);
        for (unsigned long i = 0; i < sizeof(outputs) / sizeof(outputs[0]); ++i)
            outputs[i] = response[i];
        auto targetNeuron = static_cast<unsigned>(state->takenAction());
        outputs[targetNeuron] = targetValue;
    }

    _network->train(inputs, outputs);
    return loss;
}

void NetworkTeacher::printStats(double totalLoss, unsigned epoch, unsigned age)
{
    std::cout << "epoch: " << epoch
              << "\tage: " << age
              << "\tloss: " << totalLoss
              << "\taverage loss: " << totalLoss / age << std::endl;
}

bool NetworkTeacher::serializeNetwork()
{
    try {
        _network->save(_arguments->networkFileName);
        return true;
    } catch (...) {
        return false;
    }
}


//void NetworkTeacher::performLearning(NeuralNetwork::LearningNetwork *network, const std::vector<NeuralNetwork::LearningSet> &learningSets) const
//{
//    try
//    {
//        auto connection = core::ScopedConnection(network->learningProgress.connect([] (unsigned epoch, double error) {
//            std::cout << "Epoch finished: " << epoch << ", error: " << error << std::endl;
//        }));

//        std::cout << "Learning..." << std::endl;
//        network->learn(learningSets, _arguments->learningRate, _arguments->momentum, _arguments->maxEpochs, _arguments->minError, &_sigIntCought);
//        std::cout << "Learning finished." << std::endl;
//    }
//    catch (std::invalid_argument &exception)
//    {
//        std::cerr << "Invalid argument: " << exception.what() << std::endl;
//    }
//}

}


