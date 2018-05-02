#ifndef WEBAPPLAUNCHER_H
#define WEBAPPLAUNCHER_H

#include "Application.h"
#include <Wt/WServer.h>
#include <Network.h>
#include "arguments/WebAppArguments.h"

namespace nn2048
{

class WebAppLauncher: public Application
{
public:
    WebAppLauncher(std::unique_ptr<WebAppArguments> arguments);

    int run();

protected:
    bool loadNeuralNetwork();
    void setupServer();

private:
    std::unique_ptr<WebAppArguments> _arguments;

    std::unique_ptr<Wt::WServer> _server;
    std::unique_ptr<NeuralNetwork::Network> _neuralNetwork;
};

}


#endif // WEBAPPLAUNCHER_H
