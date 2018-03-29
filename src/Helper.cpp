#include "Helper.h"
#include <iostream>
#include "utils/Defaults.h"
#include "arguments/LearningSetsMakerArguments.h"
#include "arguments/NetworkCreatorArguments.h"
#include "arguments/QLearningArguments.h"

namespace nn2048
{

void Helper::showHelp() const
{
    std::cout << "Usage: " << _execName << " [mode] [mode arguments]" << std::endl << std::endl;
    std::cout << "convert mode - converts games recorded by Bastard into learning set" << std::endl;
    std::cout << "    " << LearningSetsMakerArguments::RecordsDirectoryArgument << " directory - recordings directory" << std::endl;
    std::cout << "    " << LearningSetsMakerArguments::OutputFileNameArgument   << " output    - output file name" << std::endl;
    std::cout << "    " << LearningSetsMakerArguments::MinScoreArgument         << " score     - minimum score (optional)" << std::endl << std::endl;

    std::cout << "create mode - creates new neural network with random weights" << std::endl;
    std::cout << "    " << NetworkCreatorArguments::NetworkStructureArgument   << " structure - network structure (eg. 2,3,4 - 2 inputs, 3 hidden" << std::endl;
    std::cout << "                   neurons, 4 outputs)" << std::endl;
    std::cout << "    " << NetworkCreatorArguments::NetworkFileNameArgument    << " output    - output file name" << std::endl;
    std::cout << "    " << NetworkCreatorArguments::WeightDistributionArgument << " distrib   - weight distribution amplitude (optional, " << DefaultWeightDistribution << " by default)" << std::endl;
    std::cout << "    " << NetworkCreatorArguments::CreateFannNetworkArgument  << "           - creates FANN network instead of standard json one" << std::endl << std::endl;

    std::cout << "learn mode - performs backpropagation algorythm" << std::endl;
    std::cout << "    -n file      - neural network file name" << std::endl;
    std::cout << "    -l file      - learning sets file name" << std::endl;
    std::cout << "    -c epochs    - limit learning by maximum number of epochs" << std::endl;
    std::cout << "    -e error     - limit learning by minimum error value" << std::endl;
    std::cout << "    -r rate      - learning rate (optional, " << DefaultLearningRate << " by default)" << std::endl;
    std::cout << "    -m momentum  - momentum factor (optional, " << DefaultMomentumFactor << " by default)" << std::endl;
    std::cout << "    Arguments -c and -e can be used in combination with each other. At least" << std::endl;
    std::cout << "    one of them has to be specified." << std::endl << std::endl;

    std::cout << "qlearn mode - performs Q Learning algorythm" << std::endl;
    std::cout << "    " << QLearningArguments::NetworkFileNameArgument  << " file      - neural network file name" << std::endl;
    std::cout << "    " << QLearningArguments::MaxAgeArgument           << " max age   - limit learning by maximum number of epochs" << std::endl;
    std::cout << "    " << QLearningArguments::TargetScoreArgument      << " score     - limit learning by target score" << std::endl;
    std::cout << "    " << QLearningArguments::GammaFactorArgument      << " gamma     - gamma factor (optional, " << DefaultGammaFactor << " by default)" << std::endl;
    std::cout << "    " << QLearningArguments::LearningRateArgument     << " rate      - learning rate (optional, " << DefaultLearningRate << " by default)" << std::endl;
    std::cout << "    " << QLearningArguments::MomentumFactorArgument   << " momentum  - momentum factor (optional, " << DefaultMomentumFactor << " by default)" << std::endl;
    std::cout << "    " << QLearningArguments::EpsilonFactorArgument    << " epsilon   - epsilon factor (optional, " << DefaultEpsilonFactor << " by default)" << std::endl;
    std::cout << "    " << QLearningArguments::ReplayMemorySizeArgument << " size      - replay memory size (optional, " << DefaultReplayMemorySize << " by default)" << std::endl;
    std::cout << "    " << QLearningArguments::ReplayBatchSizeArgument  << " size      - replay batch size (optional, " << DefaultReplayBatchSize << " by default)" << std::endl << std::endl;

    std::cout << "webapp mode - launches 2048 web application" << std::endl;
    std::cout << "    -p port      - specify port to deploy app to (" << DefaultServerPort << " by default)" << std::endl;
    std::cout << "    -s servName  - server name" << std::endl;
    std::cout << "    -d docRoot   - document root" << std::endl;
    std::cout << "    -r resDir    - resources directory" << std::endl;
    std::cout << "    -a appDir    - app root directory" << std::endl;
    std::cout << "    -n netFile   - neural network file name (optional)" << std::endl;
    std::cout << "    -t score     - threshold above which games are recorded (optional, " << DefaultHighscoreToRecordThreshold << " by default)" << std::endl;
}

}


