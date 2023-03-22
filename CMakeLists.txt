cmake_minimum_required(VERSION 3.24)
project(2023-JCO-Airtime)

set(UIS src/mainfrm.ui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH C:/Qt/6.4.0/mingw_64/)
set(CMAKE_AUTOMOC TRUE)
set(QT_INSTALL_PATH C:/Qt/6.4.0/mingw_64/)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

include_directories(src)

add_executable(2023-JCO-Airtime
        src/2023-JCO-Airtime.pro
        src/gamecanvas.cpp src/gamecanvas.h
        src/gamecore.cpp src/gamecore.h
        src/gamescene.cpp src/gamescene.h
        src/gameview.cpp src/gameview.h
        src/main.cpp
        src/mainfrm.cpp src/mainfrm.h src/mainfrm.ui
        src/resources.cpp src/resources.h
        src/sprite.cpp src/sprite.h
        src/spritetickhandler.cpp src/spritetickhandler.h
        src/utilities.cpp src/utilities.h src/Player.cpp src/Player.h src/PlayerTickHandler.cpp src/PlayerTickHandler.h)

target_link_libraries(2023-JCO-Airtime
        Qt::Core
        Qt::Gui
        Qt6::Widgets
        )
