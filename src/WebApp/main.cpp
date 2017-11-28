#include <Wt/WServer.h>
#include <NeuralNetwork/networkserializer.h>
#include <fstream>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include "bastardapplication.h"

std::unique_ptr<NeuralNetwork::Network> loadNetwork()
{
    try
    {
        std::cout << "Loading neural network... ";
        std::cout.flush();
        std::ifstream file("network.json");
        if (!file.is_open())
        {
            std::cout << "failed" << std::endl;
            std::cerr << "Could not open network.json file" << std::endl;
            return nullptr;
        }

        auto network = NeuralNetwork::NetworkSerializer::deserialize(file);
        file.close();
        std::cout << "ok" << std::endl;
        return network;
    }
    catch (std::runtime_error &exception)
    {
        std::cout << "failed" << std::endl;
        std::cerr << "Runtime error during network deserialization: " << exception.what() << std::endl;
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    srand((unsigned)time(nullptr));
    auto network = loadNetwork();
    Wt::WRun(argc, argv, [&network] (const Wt::WEnvironment &env) {
        return std::make_unique<Bastard::BastardApplication>(env, network.get());
    });
    return 0;
}
