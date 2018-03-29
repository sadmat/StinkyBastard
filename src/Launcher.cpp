#include "Launcher.h"
#include <map>
#include <iostream>
#include <cstring>
#include <cmath>
#include "Helper.h"
#include "LearningSetsMaker.h"
#include "NetworkCreator.h"
#include "NetworkTeacher.h"
#include "QLearningTeacher.h"
#include "WebAppLauncher.h"
#include "utils/Defaults.h"
#include "arguments/LearningSetsMakerArgumentsParser.h"
#include "arguments/NetworkCreatorArgumentsParser.h"
#include "arguments/QLearningArgumentsParser.h"

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
    auto parser = LearningSetsMakerArgumentsParser(argc, argv);
    auto arguments = parser.parsedArguments();
    if (!arguments)
        return nullptr;
    auto pointer = dynamic_cast<LearningSetsMakerArguments *>(arguments.release());
    return std::make_unique<LearningSetsMaker>(std::unique_ptr<LearningSetsMakerArguments>(pointer));
}

std::unique_ptr<Application> Launcher::networkCreatorApplication(int argc, char *argv[])
{
    auto parser = NetworkCreatorArgumentsParser(argc, argv);
    auto arguments = parser.parsedArguments();
    if (!arguments)
        return nullptr;
    auto pointer = dynamic_cast<NetworkCreatorArguments *>(arguments.release());
    return std::make_unique<NetworkCreator>(std::unique_ptr<NetworkCreatorArguments>(pointer));
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
    auto parser = QLearningArgumentsParser(argc, argv);
    auto arguments = parser.parsedArguments();
    if (!arguments)
        return nullptr;
    auto pointer = dynamic_cast<QLearningArguments *>(arguments.release());
    return std::make_unique<QLearningTeacher>(std::unique_ptr<QLearningArguments>(pointer));
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

}


