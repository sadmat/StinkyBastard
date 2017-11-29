#include <csignal>
#include "Launcher.h"

static nn2048::Application *app = nullptr;

void onSigInt(int)
{
    if (app)
        app->onSigInt();
}

int main(int argc, char *argv[])
{
    std::signal(SIGINT, &onSigInt);
    auto application = nn2048::Launcher::application(argc, argv);
    app = application.get();
    return application->run();
}
