find_path(WT_INCLUDE_PATH Wt/WServer.h
          /usr/include
          /usr/local/include)
find_library(WT_LIBRARY wt)
find_library(WTHTTP_LIBRARY wthttp)
find_library(FANN_LIBRARY doublefann)

MESSAGE("FANN_LIBRARY ${FANN_LIBRARY}")

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

find_package(Boost REQUIRED
    filesystem
    system)

set(SOURCES
    main.cpp
    arguments/Arguments.cpp
    arguments/ArgumentParser.cpp
    utils/BoardSignalConverter.cpp
    web/GameBoardWidget.cpp
    web/GameController.cpp
    web/GameHeaderWidget.cpp
    web/GameWidget.cpp
    Helper.cpp
    web/KeyboardGameController.cpp
    Launcher.cpp
    arguments/LearningSetsMakerArguments.cpp
    arguments/LearningSetsMakerArgumentsParser.cpp
    LearningSetsMaker.cpp
    NetworkCreator.cpp
    utils/NetworkOutputConverter.cpp
    NetworkTeacher.cpp
    web/NeuralNetworkGameController.cpp
    arguments/QLearningArguments.cpp
    arguments/QLearningArgumentsParser.cpp
    utils/QLearningState.cpp
    QLearningTeacher.cpp
    utils/ReplayMemory.cpp
    web/ScoreWidget.cpp
    utils/TilePositionComparer.cpp
    web/WebApplication.cpp
    WebAppLauncher.cpp
)
            
set(HEADERS
    Application.h
    arguments/Arguments.h
    arguments/ArgumentParser.h
    utils/BoardSignalConverter.h
    utils/Defaults.h
    web/GameBoardWidget.h
    web/GameController.h
    web/GameHeaderWidget.h
    web/GameWidget.h
    Helper.h
    web/KeyboardGameController.h
    Launcher.h
    arguments/LearningSetsMakerArguments.h
    arguments/LearningSetsMakerArgumentsParser.h
    LearningSetsMaker.h
    NetworkCreator.h
    utils/NetworkOutputConverter.h
    NetworkTeacher.h
    web/NeuralNetworkGameController.h
    arguments/QLearningArguments.h
    arguments/QLearningArgumentsParser.h
    utils/QLearningState.h
    QLearningTeacher.h
    utils/ReplayMemory.h
    web/ScoreWidget.h
    utils/TilePositionComparer.h
    web/WebApplication.h
    WebAppLauncher.h
)
            
include_directories(${WT_INCLUDE_PATH})
link_directories(${JSONCPP_LIBRARY_DIRS})
add_executable(nn2048 ${SOURCES} ${HEADERS})
target_link_libraries(nn2048
    Game2048Core
    NeuralNetwork
    ${WT_LIBRARY}
    ${WTHTTP_LIBRARY}
    ${Boost_LIBRARIES}
    ${FANN_LIBRARY}
)
set_property(TARGET nn2048 PROPERTY CXX_STANDARD 14)
