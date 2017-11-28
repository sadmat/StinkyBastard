TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

macx {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib
}

INCLUDEPATH += ..
LIBS += ../TwoOhFourEightCore/libTwoOhFourEightCore.a ../NeuralNetwork/libNeuralNetwork.a -lwt -lwthttp -ljsoncpp -lz
PRE_TARGETDEPS += ../TwoOhFourEightCore/libTwoOhFourEightCore.a ../NeuralNetwork/libNeuralNetwork.a

SOURCES += main.cpp \
    bastardapplication.cpp \
    gamewidget.cpp \
    gameheaderwidget.cpp \
    gameboardwidget.cpp \
    gamecontroller.cpp \
    keyboardgamecontroller.cpp \
    tilepositioncomparer.cpp \
    scorewidget.cpp \
    boardsignalconverter.cpp \
    networkoutputconverter.cpp \
    neuralnetworkgamecontroller.cpp

HEADERS += \
    bastardapplication.h \
    gamewidget.h \
    gameheaderwidget.h \
    gameboardwidget.h \
    gamecontroller.h \
    keyboardgamecontroller.h \
    tilepositioncomparer.h \
    scorewidget.h \
    boardsignalconverter.h \
    networkoutputconverter.h \
    neuralnetworkgamecontroller.h
