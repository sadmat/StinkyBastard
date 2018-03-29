#ifndef LEARNINGSETSMAKERARGUMENTS_H
#define LEARNINGSETSMAKERARGUMENTS_H

#include "Arguments.h"
#include <string>

namespace nn2048 {

class LearningSetsMakerArguments : public Arguments
{
public:
    std::string recordsDirectory = "";
    std::string outputFileName = "";
    unsigned minScore = 0;

    const static std::string RecordsDirectoryArgument;
    const static std::string OutputFileNameArgument;
    const static std::string MinScoreArgument;
};

}


#endif // LEARNINGSETSMAKERARGUMENTS_H
