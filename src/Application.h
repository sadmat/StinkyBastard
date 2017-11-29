#ifndef APPLICATION_H
#define APPLICATION_H

#include <cstdlib>

namespace nn2048
{

class Application
{
public:
    Application() {}
    Application(const Application &other) = delete;
    Application(Application &&other) = delete;
    virtual ~Application() {}

    Application &operator=(const Application &other) = delete;
    Application &operator=(Application &&other) = delete;

    virtual int run() = 0;
    virtual void onSigInt() { std::exit(0); }
};

}

#endif // APPLICATION_H
