#ifndef LEARNINGSETSMAKERARGUMENTSPARSER_H
#define LEARNINGSETSMAKERARGUMENTSPARSER_H

#include "ArgumentParser.h"
#include <memory>

namespace nn2048 {

class LearningSetsMakerArgumentsParser : public ArgumentParser
{
public:
    LearningSetsMakerArgumentsParser(int argc, char **argv);

    std::unique_ptr<Arguments> parsedArguments();

private:
    bool parseRecordsDirectory(std::string &output);
    bool parseOutputFileName(std::string &output);
    bool parseMinimumScore(unsigned &output);
};

}


#endif // LEARNINGSETSMAKERARGUMENTSPARSER_H
