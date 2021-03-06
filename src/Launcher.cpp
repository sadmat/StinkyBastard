#include "Launcher.h"
#include <map>
#include <iostream>
#include <cstring>
#include <cmath>
#include "Helper.h"
#include "ReplayMemoryMerger.h"
#include "NetworkCreator.h"
#include "NetworkTeacher.h"
#include "QLearningTeacher.h"
#include "WebAppLauncher.h"
#include "utils/Defaults.h"
#include "arguments/ReplayMemoryMergerArgumentsParser.h"
#include "arguments/NetworkCreatorArgumentsParser.h"
#include "arguments/NetworkTeacherArgumentsParser.h"
#include "arguments/QLearningArgumentsParser.h"
#include "arguments/WebAppArgumentsParser.h"

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
        { "merge", RunMode::MergeReplayMemory },
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
    case RunMode::MergeReplayMemory:
        return replayMemoryMergerApplication(argc, argv);
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

std::unique_ptr<Application> Launcher::replayMemoryMergerApplication(int argc, char *argv[])
{
    auto parser = ReplayMemoryMergerArgumentsParser(argc, argv);
    auto arguments = parser.parsedArguments();
    if (!arguments)
        return nullptr;
    auto pointer = dynamic_cast<ReplayMemoryMergerArguments *>(arguments.release());
    return std::make_unique<ReplayMemoryMerger>(std::unique_ptr<ReplayMemoryMergerArguments>(pointer));
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
    auto parser = NetworkTeacherArgumentsParser(argc, argv);
    auto arguments = parser.parsedArguments();
    if (!arguments)
        return nullptr;
    auto pointer = dynamic_cast<NetworkTeacherArguments *>(arguments.release());
    return std::make_unique<NetworkTeacher>(std::unique_ptr<NetworkTeacherArguments>(pointer));
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
    auto parser = WebAppArgumentsParser(argc, argv);
    auto arguments = parser.parsedArguments();
    if (!arguments)
        return nullptr;
    auto pointer = dynamic_cast<WebAppArguments *>(arguments.release());
    return std::make_unique<WebAppLauncher>(std::unique_ptr<WebAppArguments>(pointer));
}

}


