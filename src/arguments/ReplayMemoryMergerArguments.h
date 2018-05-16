#ifndef REPLAYMEMORYMERGERARGUMENTS_H
#define REPLAYMEMORYMERGERARGUMENTS_H

#include "Arguments.h"
#include <string>

namespace nn2048 {

class ReplayMemoryMergerArguments : public Arguments
{
public:
    std::string inputDirectory;
    std::string outputFileName;

    const static std::string InputDirectoryArgument;
    const static std::string OutputFileNameArgument;
};

}


#endif // REPLAYMEMORYMERGERARGUMENTS_H
