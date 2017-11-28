#include <csignal>
#include "launcher.h"

static Stinky::Application *app = nullptr;

void onSigInt(int)
{
    if (app)
        app->onSigInt();
}

int main(int argc, char *argv[])
{
    std::signal(SIGINT, &onSigInt);
    auto application = Stinky::Launcher::application(argc, argv);
    app = application.get();
    return application->run();
}
