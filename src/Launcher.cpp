#include "Launcher.h"
#include <map>
#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include "Helper.h"
#include "LearningSetsMaker.h"
#include "NetworkCreator.h"
#include "NetworkTeacher.h"
#include "QLearningTeacher.h"
#include "WebAppLauncher.h"
#include "utils/Defaults.h"

namespace nn2048
{

std::unique_ptr<Application> Launcher::application(int argc, char *argv[])
{
    if (argc < 2)
        return helperApplication(argv[0]);
    RunMode mode = parseRunMode(argv[1]);
    auto app = applicationForRunMode(mode, argc, argv);
    return app ? std::move(app) : helperApplication(argv[0]);
}

RunMode Launcher::parseRunMode(const std::string &mode)
{
    static std::map<std::string, RunMode> dictionary {
        { "convert", RunMode::MakeLearningSets },
        { "create", RunMode::CreateNetwork },
        { "learn", RunMode::NetworkLearning },
        { "qlearn", RunMode::QNetworkLearning },
        { "webapp", RunMode::WebApp }
    };
    return dictionary[mode];
}

std::unique_ptr<Application> Launcher::applicationForRunMode(RunMode mode, int argc, char *argv[])
{
    switch (mode)
    {
    case RunMode::MakeLearningSets:
        return setsMakerApplication(argc, argv);
    case RunMode::CreateNetwork:
        return networkCreatorApplication(argc, argv);
    case RunMode::NetworkLearning:
        return networkTeacherApplication(argc, argv);
    case RunMode::QNetworkLearning:
        return qNetworkTeacherApplication(argc, argv);
    case RunMode::WebApp:
        return webApplication(argc, argv);
    case RunMode::HelpMode:
    default:
        return helperApplication(argv[0]);
    }
}

std::unique_ptr<Application> Launcher::helperApplication(const std::string &execName)
{
    return std::make_unique<Helper>(execName);
}

std::unique_ptr<Application> Launcher::setsMakerApplication(int argc, char *argv[])
{
    std::string recordsDirectory;
    std::string outputFileName;
    unsigned minScore = 0;

    for (unsigned i = 2; i < argc; ++i)
    {
        // Recordings directory
        if (strcmp("-d", argv[i]) == 0)
        {
            if (!recordsDirectory.empty())
            {
                std::cerr << "Recordings directory has to be specified once only" << std::endl;
                return nullptr;
            }
            if (argc <= i + 1)
            {
                std::cerr << "Recordings directory argument requires parameter" << std::endl;
                return nullptr;
            }
            recordsDirectory = argv[++i];
        }
        // Output file name
        else if (strcmp("-o", argv[i]) == 0)
        {
            if (!outputFileName.empty())
            {
                std::cerr << "Output file name has to be specified once only" << std::endl;
                return nullptr;
            }
            if (argc <= i + 1)
            {
                std::cerr << "Output file name argument requires parameter" << std::endl;
                return nullptr;
            }
            outputFileName = argv[++i];
        }
        // Min score
        else if (strcmp("-s", argv[i]) == 0)
        {
            if (argc <= i + 1)
            {
                std::cerr << "Score arguments requires parameter" << std::endl;
                return nullptr;
            }
            try
            {
                int score = std::stoi(argv[++i]);
                if (score < 0)
                    throw std::invalid_argument("negative score");
                minScore = score;
            }
            catch (const std::invalid_argument &ex)
            {
                std::cerr << "Score argument has to be positive integer" << std::endl;
                return nullptr;
            }
        }
        else
        {
            std::cerr << "Unknown argument: " << argv[i] << std::endl;
            return nullptr;
        }
    }
    if (recordsDirectory.empty())
    {
        std::cerr << "Recordings directory not specified" << std::endl;
        return nullptr;
    }
    if (outputFileName.empty())
    {
        std::cerr << "Output file name not specified" << std::endl;
        return nullptr;
    }
    return std::make_unique<LearningSetsMaker>(recordsDirectory, outputFileName, minScore);
}

std::unique_ptr<Application> Launcher::networkCreatorApplication(int argc, char *argv[])
{
    std::string structureString;
    std::string fileName;
    double distributionAmplitude = DefaultWeightDistribution;

    for (unsigned i = 2; i < argc; ++i)
    {
        if (strcmp("-s", argv[i]) == 0)
        {
            if (!structureString.empty())
            {
                std::cerr << "Structure has to be specified once only" << std::endl;
                return nullptr;
            }
            if (argc <= i + 1)
            {
                std::cerr << "Structure argument requires parameter" << std::endl;
                return nullptr;
            }
            structureString = argv[++i];
        }
        else if (strcmp("-o", argv[i]) == 0)
        {
            if (!fileName.empty())
            {
                std::cerr << "Output file name has to be specified once only" << std::endl;
                return nullptr;
            }
            if (argc <= i + 1)
            {
                std::cerr << "Output file name argument requires parameter" << std::endl;
                return nullptr;
            }
            fileName = argv[++i];
        }
        else if (strcmp("-d", argv[i]) == 0)
        {
            if (argc <= i + 1)
            {
                std::cerr << "Weight distribution amplitude argument requires parameter" << std::endl;
                return nullptr;
            }
            try
            {
                distributionAmplitude = std::stod(argv[++i]);
                if (distributionAmplitude <= 0.0)
                    throw std::invalid_argument("Weight distribution amplitude has to be positive real value");
            }
            catch (std::invalid_argument &exception)
            {
                std::cerr << "Weight distribution amplitude parsing failed: " << exception.what() << std::endl;
                return nullptr;
            }
        }
        else
        {
            std::cerr << "Unknown parameter: " << argv[i];
            return nullptr;
        }
    }

    if (structureString.empty())
    {
        std::cerr << "Structure argument not set" << std::endl;
        return nullptr;
    }
    if (fileName.empty())
    {
        std::cerr << "Output file name argument not set" << std::endl;
        return nullptr;
    }

    auto structureStringValues = splitString(structureString, ',');
    std::vector<unsigned> structure;
    structure.reserve(structureStringValues.size());
    for (auto &stringValue: structureStringValues)
    {
        unsigned value;
        try
        {
            value = std::stoi(stringValue);
        }
        catch (std::invalid_argument &exception)
        {
            std::cerr << "Invalid structure value at: " << stringValue << std::endl;
            return nullptr;
        }
        structure.push_back(value);
    }
    return std::make_unique<NetworkCreator>(structure, fileName, distributionAmplitude);
}

std::unique_ptr<Application> Launcher::networkTeacherApplication(int argc, char *argv[])
{
    std::string networkFileName;
    std::string learningSetsFileName;
    unsigned maxEpochs = 0;
    double minError = 0.0;
    double learningRate = DefaultLearningRate;
    double momentum = DefaultMomentumFactor;

    for (int i = 2; i < argc; ++i)
    {
        // Neuron file name
        if (strcmp("-n", argv[i]) == 0)
        {
            if (!networkFileName.empty())
            {
                std::cerr << "Network file name already set" << std::endl;
                return nullptr;
            }
            if (argc <= i + 1)
            {
                std::cerr << "Network file name argument requires parameter" << std::endl;
                return nullptr;
            }
            networkFileName = argv[++i];
        }
        // Learning sets file name
        else if (strcmp("-l", argv[i]) == 0)
        {
            if (!learningSetsFileName.empty())
            {
                std::cerr << "Learning sets file name already set" << std::endl;
                return nullptr;
            }
            if (argc <= i + 1)
            {
                std::cerr << "Learning sets file name argument requires parameter" << std::endl;
                return nullptr;
            }
            learningSetsFileName = argv[++i];
        }
        // Epochs
        else if (strcmp("-c", argv[i]) == 0)
        {
            if (maxEpochs)
            {
                std::cerr << "Epochs limit already set" << std::endl;
                return nullptr;
            }
            if (argc < i + 1)
            {
                std::cerr << "Epochs limit argument requires parameter" << std::endl;
                return nullptr;
            }
            try
            {
                maxEpochs = std::stoi(argv[++i]);
            }
            catch (std::invalid_argument &exception)
            {
                std::cerr << "Invalid epochs limit value: " << argv[i] << std::endl;
                return nullptr;
            }
        }
        // Error
        else if (strcmp("-e", argv[i]) == 0)
        {
            if (std::abs(minError) > 0.0000001)
            {
                std::cerr << "Error limit already set" << std::endl;
                return nullptr;
            }
            if (argc <= i + 1)
            {
                std::cerr << "Error limit argument requires parameter" << std::endl;
                return nullptr;
            }
            try
            {
                minError = std::stod(argv[++i]);
            }
            catch (std::runtime_error &exception)
            {
                std::cerr << "Invalid error limit value: " << argv[i] << std::endl;
            }
        }
        // Learning rate
        else if (strcmp("-r", argv[i]) == 0)
        {
            if (argc <= i + 1)
            {
                std::cerr << "Learning rate argument requires parameter" << std::endl;
                return nullptr;
            }
            try
            {
                learningRate = std::stod(argv[++i]);
            }
            catch (std::runtime_error &exception)
            {
                std::cerr << "Invalid learning rate value: " << argv[i] << std::endl;
                return nullptr;
            }
        }
        // Momentum
        else if (strcmp("-m", argv[i]) == 0)
        {
            if (argc <= i + 1)
            {
                std::cerr << "Momentum argument requires parameter" << std::endl;
                return nullptr;
            }
            try
            {
                momentum = std::stod(argv[++i]);
            }
            catch (std::runtime_error &exception)
            {
                std::cerr << "Momentum argument requires parameter" << std::endl;
                return nullptr;
            }
        }
        else
        {
            std::cerr << "Unknown argument: " << argv[i] << std::endl;
            return nullptr;
        }
    }

    if (networkFileName.empty())
    {
        std::cerr << "Network file name not specified" << std::endl;
        return nullptr;
    }
    if (learningSetsFileName.empty())
    {
        std::cerr << "Learning sets file name not specified" << std::endl;
        return nullptr;
    }
    if (maxEpochs == 0 && std::abs(minError) < 0.000001)
    {
        std::cerr << "Specify at least one limit" << std::endl;
        return nullptr;
    }

    return std::make_unique<NetworkTeacher>(networkFileName, learningSetsFileName, maxEpochs, minError, learningRate, momentum);
}

std::unique_ptr<Application> Launcher::qNetworkTeacherApplication(int argc, char *argv[])
{
    std::string networkFileName;
    unsigned maxEpochs = 0;
    unsigned targetScore = 0;
    double gamma = DefaultGammaFactor;
    double learningRate = DefaultLearningRate;
    double momentum = DefaultMomentumFactor;

    for (int i = 2; i < argc; ++i)
    {
        // Neuron file name
        if (strcmp("-n", argv[i]) == 0)
        {
            if (!networkFileName.empty())
            {
                std::cerr << "Network file name already set" << std::endl;
                return nullptr;
            }
            if (argc <= i + 1)
            {
                std::cerr << "Network file name argument requires parameter" << std::endl;
                return nullptr;
            }
            networkFileName = argv[++i];
        }
        // Epochs
        else if (strcmp("-e", argv[i]) == 0)
        {
            if (maxEpochs)
            {
                std::cerr << "Epochs limit already set" << std::endl;
                return nullptr;
            }
            if (argc < i + 1)
            {
                std::cerr << "Epochs limit argument requires parameter" << std::endl;
                return nullptr;
            }
            try
            {
                maxEpochs = std::stoi(argv[++i]);
            }
            catch (std::invalid_argument &exception)
            {
                std::cerr << "Invalid epochs limit value: " << argv[i] << std::endl;
                return nullptr;
            }
        }
        else if (strcmp("-s", argv[i]) == 0)
        {
            if (targetScore)
            {
                std::cerr << "Target score already set" << std::endl;
                return nullptr;
            }
            if (argc < i + 1)
            {
                std::cerr << "Target score argument requires parameter" << std::endl;
                return nullptr;
            }
            try
            {
                targetScore = std::stoi(argv[++i]);
            }
            catch (std::invalid_argument &exception)
            {
                std::cerr << "Invalid target score value: " << argv[i] << std::endl;
                return nullptr;
            }
        }
        // Gamma
        else if (strcmp("-g", argv[i]) == 0)
        {
            if (argc <= i + 1)
            {
                std::cerr << "Gamma factor argument requires parameter" << std::endl;
                return nullptr;
            }
            try
            {
                gamma = std::stod(argv[++i]);
            }
            catch (std::runtime_error &exception)
            {
                std::cerr << "Invalid gamma factor value: " << argv[i] << std::endl;
            }
        }
        // Learning rate
        else if (strcmp("-r", argv[i]) == 0)
        {
            if (argc <= i + 1)
            {
                std::cerr << "Learning rate argument requires parameter" << std::endl;
                return nullptr;
            }
            try
            {
                learningRate = std::stod(argv[++i]);
            }
            catch (std::runtime_error &exception)
            {
                std::cerr << "Invalid learning rate value: " << argv[i] << std::endl;
                return nullptr;
            }
        }
        // Momentum
        else if (strcmp("-m", argv[i]) == 0)
        {
            if (argc <= i + 1)
            {
                std::cerr << "Momentum argument requires parameter" << std::endl;
                return nullptr;
            }
            try
            {
                momentum = std::stod(argv[++i]);
            }
            catch (std::runtime_error &exception)
            {
                std::cerr << "Momentum argument requires parameter" << std::endl;
                return nullptr;
            }
        }
        else
        {
            std::cerr << "Unknown argument: " << argv[i] << std::endl;
            return nullptr;
        }
    }

    if (networkFileName.empty())
    {
        std::cerr << "Network file name not specified" << std::endl;
        return nullptr;
    }
    if (maxEpochs == 0 && targetScore == 0)
    {
        std::cerr << "Neither epoch limit nor target score values are set." << std::endl;
        return nullptr;
    }

    return std::make_unique<QLearningTeacher>(networkFileName, maxEpochs, targetScore, gamma, learningRate, momentum);
}

std::unique_ptr<Application> Launcher::webApplication(int argc, char *argv[])
{
    unsigned short port = DefaultServerPort;
    std::string portString;
    std::string serverName;
    std::string documentRoot;
    std::string resourcesDirectory;
    std::string appRootDirectory;
    std::string neuralNetworkFileName;
    unsigned long highscoreThreshold = DefaultHighscoreToRecordThreshold;
    std::string highscoreThresholdString;

    for (int i = 2; i < argc; ++i)
    {
        // Port
        if (!strcmp("-p", argv[i]))
        {
            if (!portString.empty())
            {
                std::cerr << "Port number cannot be specified twice" << std::endl;
                return nullptr;
            }
            else if (argc <= i + 1)
            {
                std::cerr << "Port number argument requires parameter" << std::endl;
                return nullptr;
            }
            portString = argv[++i];
        }
        // Server name
        else if (!strcmp("-s", argv[i]))
        {
            if (!serverName.empty())
            {
                std::cerr << "Server name cannot be set twice" << std::endl;
                return nullptr;
            }
            else if (argc <= i + 1)
            {
                std::cerr << "Server name argument requires parameter" << std::endl;
                return nullptr;
            }
            serverName = argv[++i];
        }
        // Doc root
        else if (!strcmp("-d", argv[i]))
        {
            if (!documentRoot.empty())
            {
                std::cerr << "Document root cannot be set twice" << std::endl;
                return nullptr;
            }
            else if (argc <= i + 1)
            {
                std::cerr << "Document root argument requires parameter" << std::endl;
                return nullptr;
            }
            documentRoot = argv[++i];
        }
        // Resources directory
        else if (!strcmp("-r", argv[i]))
        {
            if (!resourcesDirectory.empty())
            {
                std::cerr << "Resources directory cannot be set twice" << std::endl;
                return nullptr;
            }
            else if (argc <= i + 1)
            {
                std::cerr << "Resources directory argument requires parameter" << std::endl;
                return nullptr;
            }
            resourcesDirectory = argv[++i];
        }
        // App root directory
        else if (!strcmp("-a", argv[i]))
        {
            if (!appRootDirectory.empty())
            {
                std::cerr << "Application root directory cannot be set twice" << std::endl;
                return nullptr;
            }
            else if (argc <= i + 1)
            {
                std::cerr << "Application root directory argument requires paramter" << std::endl;
                return nullptr;
            }
            appRootDirectory = argv[++i];
        }
        // Neural network file name
        else if (!strcmp("-n", argv[i]))
        {
            if (!neuralNetworkFileName.empty())
            {
                std::cerr << "Neural network file name cannot be set twice" << std::endl;
                return nullptr;
            }
            else if (argc <= i + 1)
            {
                std::cerr << "Neural network file name argument requires parameter" << std::endl;
                return nullptr;
            }
            neuralNetworkFileName = argv[++i];
        }
        // Highscore threshold
        else if (!strcmp("-t", argv[i]))
        {
            if (!highscoreThresholdString.empty())
            {
                std::cerr << "Highscore threshold cannot be set twice" << std::endl;
                return nullptr;
            }
            else if (argc <= i + 1)
            {
                std::cerr << "Highscore threshold argument requires parameter" << std::endl;
                return nullptr;
            }
            highscoreThresholdString = argv[++i];
        }
        // Unknown argument
        else
        {
            std::cerr << "Unknown argument: " << argv[i] << std::endl;
            return nullptr;
        }
    }

    if (!portString.empty())
    {
        try
        {
            int parsed = std::stoi(portString);
            if (parsed <= 0 || parsed >= 65536)
                throw std::invalid_argument("Port number has to be in range [1..65535]");
            port = (unsigned short)parsed;
        }
        catch (std::invalid_argument &exception)
        {
            std::cerr << "Port number parsing error: " << exception.what() << std::endl;
            return nullptr;
        }
    }

    if (!highscoreThresholdString.empty())
    {
        try
        {
            highscoreThreshold = std::stoul(highscoreThresholdString);
        }
        catch (std::invalid_argument &exception)
        {
            std::cerr << "Error parsing highscore threshold: " << exception.what();
            return nullptr;
        }
    }

    if (serverName.empty())
    {
        std::cerr << "Server name not specified" << std::endl;
        return nullptr;
    }
    else if (documentRoot.empty())
    {
        std::cerr << "Document root not specified" << std::endl;
        return nullptr;
    }
    else if (resourcesDirectory.empty())
    {
        std::cerr << "Resources directory not specified" << std::endl;
        return nullptr;
    }
    else if (appRootDirectory.empty())
    {
        std::cerr << "Application root directory not specified" << std::endl;
    }

    return std::make_unique<WebAppLauncher>(
            argv[0],
            port,
            serverName,
            documentRoot,
            resourcesDirectory,
            appRootDirectory,
            neuralNetworkFileName,
            highscoreThreshold);
}

std::vector<std::string> Launcher::splitString(const std::string &string, char delimiter)
{
    std::stringstream stream(string);
    std::string item;
    std::vector<std::string> elements;
    while (std::getline(stream, item, delimiter))
    {
        elements.push_back(std::move(item));
    }
    return elements;
}

}


