#ifndef ARGUMENTPARSER_H
#define ARGUMENTPARSER_H

#include <memory>
#include "Arguments.h"

namespace nn2048 {

class ArgumentParser
{
public:
    ArgumentParser(int argc, char **argv, unsigned startIndex);
    virtual ~ArgumentParser();

    virtual std::unique_ptr<Arguments> parsedArguments() = 0;

protected:
    bool hasParameter(unsigned index) const;
    bool tryParseUnsigned(const char *param, unsigned &out) const;
    bool tryParseDouble(const char *param, double &out) const;

protected:
    int _argc;
    char **_argv;
    unsigned _startIndex;
    unsigned _currentArgIndex;
};

}

#endif // ARGUMENTPARSER_H
