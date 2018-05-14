#include "ArgumentParser.h"
#include <string>
#include <iostream>
#include <cstring>

namespace nn2048 {

ArgumentParser::ArgumentParser(int argc, char **argv, unsigned startIndex) :
    _argc(argc),
    _argv(argv),
    _startIndex(startIndex),
    _currentArgIndex(startIndex)
{}

ArgumentParser::~ArgumentParser()
{}

bool ArgumentParser::hasParameter(unsigned index) const
{
    return index + 1 < static_cast<unsigned>(_argc);
}

bool ArgumentParser::tryParseUnsigned(const char *param, unsigned &out) const
{
    try {
        size_t index = 0;
        out = static_cast<unsigned>(std::stoul(param, &index));
        if (index < strlen(param))
            return false;
    } catch (std::invalid_argument ex) {
        return false;
    } catch (std::out_of_range ex) {
        return false;
    }
    return true;
}

bool ArgumentParser::tryParseDouble(const char *param, double &out) const
{
    try {
        size_t index = 0;
        out = std::stod(param, &index);
        if (index < strlen(param))
            return false;
    } catch (std::invalid_argument ex) {
        std::cerr << "Invalid argument exception: " << ex.what() << std::endl;
        return false;
    } catch (std::out_of_range ex) {
        std::cerr << "Out of range exception: " << ex.what() << std::endl;
        return false;
    }
    return true;
}

}
