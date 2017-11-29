#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <string>
#include <memory>
#include <vector>
#include "Application.h"

namespace nn2048
{

enum class RunMode
{
    HelpMode,
    MakeLearningSets,
    CreateNetwork,
    NetworkLearning
};

class Launcher
{
public:
    static std::unique_ptr<Application> application(int argc, char *argv[]);

protected:
    static RunMode parseRunMode(const std::string &mode);
    static std::unique_ptr<Application> applicationForRunMode(RunMode mode, int argc, char *argv[]);
    static std::unique_ptr<Application> helperApplication(const std::string &execName);
    static std::unique_ptr<Application> setsMakerApplication(int argc, char *argv[]);
    static std::unique_ptr<Application> networkCreatorApplication(int argc, char *argv[]);
    static std::unique_ptr<Application> networkTeacherApplication(int argc, char *argv[]);

    static std::vector<std::string> splitString(const std::string &string, char delimiter);
};

}



#endif // LAUNCHER_H
