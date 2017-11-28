set(SOURCES helper.cpp
            bastardapplication.cpp
            boardsignalconverter.cpp
            gameboardwidget.cpp
            gamecontroller.cpp
            gameheaderwidget.cpp
            gamewidget.cpp
            keyboardgamecontroller.cpp
            launcher.cpp
            learningsetsmaker.cpp
            networkcreator.cpp
            networkoutputconverter.cpp
            networkteacher.cpp
            neuralnetworkgamecontroller.cpp
            scorewidget.cpp
            tilepositioncomparer.cpp)
            
set(HEADERS application.h
            bastardapplication.h
            boardsignalconverter.h
            gameboardwidget.h
            gamecontroller.h
            gameheaderwidget.h
            gamewidget.h
            keyboardgamecontroller.h
            launcher.h
            learningsetsmaker.h
            networkcreator.h
            networkoutputconverter.h
            networkteacher.h
            neuralnetworkgamecontroller.h
            scorewidget.h
            tilepositioncomparer.h)
            
add_executable(nn2048 ${SOURCES} ${HEADERS})
