find_path(WT_INCLUDE_PATH Wt/WServer.h
          /usr/include
          /usr/local/include)
find_library(WT_LIBRARY wt)
find_library(WTHTTP_LIBRARY wthttp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

find_package(Boost REQUIRED
    filesystem
    system)

set(SOURCES
    main.cpp
    bastardapplication.cpp
    boardsignalconverter.cpp
    gameboardwidget.cpp
    gamecontroller.cpp
    gameheaderwidget.cpp
    gamewidget.cpp
    helper.cpp
    keyboardgamecontroller.cpp
    launcher.cpp
    learningsetsmaker.cpp
    networkcreator.cpp
    networkoutputconverter.cpp
    networkteacher.cpp
    neuralnetworkgamecontroller.cpp
    scorewidget.cpp
    tilepositioncomparer.cpp)
            
set(HEADERS
    application.h
    bastardapplication.h
    boardsignalconverter.h
    gameboardwidget.h
    gamecontroller.h
    gameheaderwidget.h
    gamewidget.h
    helper.h
    keyboardgamecontroller.h
    launcher.h
    learningsetsmaker.h
    networkcreator.h
    networkoutputconverter.h
    networkteacher.h
    neuralnetworkgamecontroller.h
    scorewidget.h
    tilepositioncomparer.h)
            
include_directories(${WT_INCLUDE_PATH})
link_directories(${JSONCPP_LIBRARY_DIRS})
add_executable(nn2048 ${SOURCES} ${HEADERS})
target_link_libraries(nn2048
    Game2048Core
    NeuralNetwork
    ${WT_LIBRARY}
    ${WT_HTTP_LIBRARY}
    ${Boost_LIBRARIES})
set_property(TARGET nn2048 PROPERTY CXX_STANDARD 14)
