#include "Helper.h"
#include <iostream>

namespace nn2048
{

void Helper::showHelp() const
{
    std::cout << "Usage: " << _execName << " [mode] [mode arguments]" << std::endl;
    std::cout << "convert mode - converts games recorded by Bastard into learning set" << std::endl;
    std::cout << "    -d directory - recordings directory" << std::endl;
    std::cout << "    -o output    - output file name" << std::endl;
    std::cout << "    -s score     - minimum score (optional)" << std::endl;
    std::cout << "create mode - creates new neural network with random weights" << std::endl;
    std::cout << "    -s structure - network structure (eg. 2,3,4 - 2 inputs, 3 hidden" << std::endl;
    std::cout << "                   neurons, 4 outputs)" << std::endl;
    std::cout << "    -o output    - output file name" << std::endl;
    std::cout << "    -d distrib   - weight distribution amplitude (optional, 0.02 by default)" << std::endl;
    std::cout << "learn mode - performs backpropagation algorythm" << std::endl;
    std::cout << "    -n file      - neural network file name" << std::endl;
    std::cout << "    -l file      - learning sets file name" << std::endl;
    std::cout << "    -c epochs    - limit learning by maximum number of epochs" << std::endl;
    std::cout << "    -e error     - limit learning by minimum error value" << std::endl;
    std::cout << "    -r rate      - learning rate (optional, 0.08 by default)" << std::endl;
    std::cout << "    -m momentum  - momentum factor (optional, 0.02 by default)" << std::endl;
    std::cout << "    Arguments -c and -e can be used in combination with each other. At least" << std::endl;
    std::cout << "    one of them has to be specified." << std::endl;
    std::cout << "webapp mode - launches 2048 web application" << std::endl;
    std::cout << "    -p port      - specify port to deploy app to (4000 by default)" << std::endl;
    std::cout << "    -s name      - server name" << std::endl;
    std::cout << "    -d docroot   - document root" << std::endl;
    std::cout << "    -r dir       - resources directory" << std::endl;
    std::cout << "    -a dir       - app root directory" << std::endl;
    std::cout << "    -n file      - neural network file name (optional)" << std::endl;
}

}


