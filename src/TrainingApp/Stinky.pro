TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

macx {
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib
}

INCLUDEPATH += ..
LIBS += ../TwoOhFourEightCore/libTwoOhFourEightCore.a ../NeuralNetwork/libNeuralNetwork.a -ljsoncpp -lboost_filesystem -lboost_system
PRE_TARGETDEPS += ../TwoOhFourEightCore/libTwoOhFourEightCore.a ../NeuralNetwork/libNeuralNetwork.a

SOURCES += main.cpp \
    launcher.cpp \
    learningsetsmaker.cpp \
    networkcreator.cpp \
    networkteacher.cpp \
    helper.cpp

HEADERS += \
    launcher.h \
    learningsetsmaker.h \
    networkcreator.h \
    networkteacher.h \
    helper.h \
    application.h

