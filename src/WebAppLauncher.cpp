#include "WebAppLauncher.h"
#include <iostream>
#include <fstream>
#include <NetworkSerializer.h>
#include "web/WebApplication.h"

namespace nn2048
{

WebAppLauncher::WebAppLauncher(const std::string &execName,
                               unsigned short port,
                               const std::string &serverName,
                               const std::string &documentRoot,
                               const std::string &resourcesDirectory,
                               const std::string &appRootDirectory,
                               const std::string &neuralNetworkFileName):
    _execName(execName),
    _port(port),
    _serverName(serverName),
    _documentRoot(documentRoot),
    _resourcesDirectory(resourcesDirectory),
    _appRootDirectory(appRootDirectory),
    _neuralNetworkFileName(neuralNetworkFileName)
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
    if (_neuralNetworkFileName.empty())
        return true;
    try
    {
        std::ifstream file(_neuralNetworkFileName);
        if (!file.is_open())
        {
            std::cerr << "Neural network file could not be opened (" << _neuralNetworkFileName << ")" << std::endl;
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
    std::string portString = std::to_string(_port);
    const char *argv[] {
        _execName.c_str(),
        "--http-address", "0.0.0.0",
        "--http-port", portString.c_str(),
        "--servername", _serverName.c_str(),
        "--docroot", _documentRoot.c_str(),
        "--resources-dir", _resourcesDirectory.c_str(),
        "--approot", _appRootDirectory.c_str()
    };
    int argc = sizeof(argv) / sizeof(char*);
    _server = std::make_unique<Wt::WServer>(argc, const_cast<char **>(argv));
    _server->addEntryPoint(Wt::EntryPointType::Application,
                           [this] (const Wt::WEnvironment &environment) {
        return std::make_unique<WebApplication>(environment, _neuralNetwork.get());
    });
}

}
