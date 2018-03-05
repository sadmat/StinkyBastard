#include <csignal>
#include <cstdlib>
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
    std::srand(static_cast<unsigned int>(time(nullptr)));
    auto application = nn2048::Launcher::application(argc, argv);
    app = application.get();
    return application->run();
}
