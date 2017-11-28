#include "launcher.h"
#include <map>
#include <iostream>
#include <cstring>
#include <sstream>
#include <cmath>
#include "helper.h"
#include "learningsetsmaker.h"
#include "networkcreator.h"
#include "networkteacher.h"

namespace Stinky
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
        { "learn", RunMode::NetworkLearning }
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
    double distributionAmplitude = 0.2;

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
    double learningRate = 0.08;
    double momentum = 0.02;

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


