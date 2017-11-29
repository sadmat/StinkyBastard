#ifndef WEBAPPLAUNCHER_H
#define WEBAPPLAUNCHER_H

#include "Application.h"
#include <Wt/WServer.h>
#include <Network.h>

namespace nn2048
{

class WebAppLauncher: public Application
{
public:
    WebAppLauncher(const std::string &execName,
                   unsigned short port,
                   const std::string &serverName,
                   const std::string &documentRoot,
                   const std::string &resourcesDirectory,
                   const std::string &appRootDirectory,
                   const std::string &neuralNetworkFileName);

    int run();

protected:
    bool loadNeuralNetwork();
    void setupServer();

private:
    std::string _execName;
    unsigned short _port;
    std::string _serverName;
    std::string _documentRoot;
    std::string _resourcesDirectory;
    std::string _appRootDirectory;
    std::string _neuralNetworkFileName;

    std::unique_ptr<Wt::WServer> _server;
    std::unique_ptr<NeuralNetwork::Network> _neuralNetwork;
};

}


#endif // WEBAPPLAUNCHER_H
