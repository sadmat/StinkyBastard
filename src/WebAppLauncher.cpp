#include "WebAppLauncher.h"
#include <iostream>
#include <fstream>
#include <NetworkSerializer.h>
#include "web/WebApplication.h"

namespace nn2048
{

WebAppLauncher::WebAppLauncher(std::unique_ptr<WebAppArguments> arguments):
    _arguments(std::move(arguments))
{}

int WebAppLauncher::run()
{
    try
    {
        loadNeuralNetwork();
        setupServer();
        if (_server->start())
        {
            _server->waitForShutdown();
            _server->stop();
        }
        else
        {
            std::cerr << "There was a problem starting server" << std::endl;
            return -1;
        }
    }
    catch (Wt::WServer::Exception &exception)
    {
        std::cerr << "Wt::WServer exception caught: " << exception.what() << std::endl;
        return -1;
    }
    return 0;
}

bool WebAppLauncher::loadNeuralNetwork()
{
    if (_arguments->neuralNetworkFileName.empty())
        return true;
    try
    {
        std::ifstream file(_arguments->neuralNetworkFileName);
        if (!file.is_open())
        {
            std::cerr << "Neural network file could not be opened (" << _arguments->neuralNetworkFileName << ")" << std::endl;
            return false;
        }
        _neuralNetwork = NeuralNetwork::NetworkSerializer::deserialize(file);
        file.close();
    }
    catch (std::runtime_error &exception)
    {
        std::cerr << "Error during neural network deserialization: " << exception.what() << std::endl;
        return false;
    }
    return true;
}

void WebAppLauncher::setupServer()
{
    std::string portString = std::to_string(_arguments->port);
    const char *argv[] {
        _arguments->argv0.c_str(),
        "--http-address", "0.0.0.0",
        "--http-port", portString.c_str(),
        "--servername", _arguments->serverName.c_str(),
        "--docroot", _arguments->documentRoot.c_str(),
        "--resources-dir", _arguments->resourcesDirectory.c_str(),
        "--approot", _arguments->appRootDirectory.c_str()
    };
    int argc = sizeof(argv) / sizeof(char*);
    _server = std::make_unique<Wt::WServer>(argc, const_cast<char **>(argv));
    _server->addEntryPoint(Wt::EntryPointType::Application,
                           [this] (const Wt::WEnvironment &environment) {
        return std::make_unique<WebApplication>(environment, _neuralNetwork.get(), _arguments->highscoreThreshold);
    });
}

}
