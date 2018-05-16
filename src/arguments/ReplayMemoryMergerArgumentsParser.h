#ifndef REPLAYMEMORYMERGERARGUMENTSPARSER_H
#define REPLAYMEMORYMERGERARGUMENTSPARSER_H

#include "ArgumentParser.h"

namespace nn2048 {

class ReplayMemoryMergerArgumentsParser : public ArgumentParser
{
public:
    ReplayMemoryMergerArgumentsParser(int argc, char **argv);

    std::unique_ptr<Arguments> parsedArguments();

private:
    bool parseInputDirectory(std::string &output);
    bool parseOutputFileName(std::string &output);
};

}


#endif // REPLAYMEMORYMERGERARGUMENTSPARSER_H
