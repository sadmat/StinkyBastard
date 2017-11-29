cmake_minimum_required(VERSION 3.1)
project(nn2048)

add_subdirectory(modules)

include_directories(modules/Game2048Core/src
                    modules/NeuralNetwork/src
                    ${WT_INCLUDE_PATH})
add_subdirectory(src)
