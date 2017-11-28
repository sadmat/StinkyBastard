#ifndef HELPER_H
#define HELPER_H

#include "application.h"
#include <string>

namespace Stinky
{

class Helper: public Application
{
public:
    Helper(const std::string &execName): _execName(execName) {}

    int run() { showHelp(); return 0; }

protected:
    void showHelp() const;

private:
    std::string _execName;
};

}


#endif // HELPER_H
