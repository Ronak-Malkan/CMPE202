cmake_minimum_required(VERSION 3.16)

project(RPS_Project VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the src folder to the include path so that headers in src/ can be found.
include_directories(${CMAKE_SOURCE_DIR}/src)

# --- Build the Console Version ---
set(CONSOLE_SOURCES
    src/main.cpp
    src/ComputerPlayer.h
    src/Game.h
    src/HumanPlayer.h
    src/Move.h
    src/Player.h
    src/RandomStrategy.h
    src/SmartStrategy.h
    src/Strategy.h
)

add_executable(rps_console ${CONSOLE_SOURCES})

# --- Build the GUI Version ---
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(GUI_SOURCES
    gui/main_gui.cpp
    gui/mainwindow.cpp
    gui/mainwindow.h
    gui/RPSGameManager.cpp
    gui/RPSGameManager.h
    # Also include the RPS logic headers from src/ as needed.
    src/ComputerPlayer.h
    src/Game.h
    src/HumanPlayer.h
    src/Move.h
    src/Player.h
    src/RandomStrategy.h
    src/SmartStrategy.h
    src/Strategy.h
)

add_executable(rps_gui ${GUI_SOURCES})

# Enable AUTOMOC so that files with Q_OBJECT macros are processed correctly.
set_target_properties(rps_gui PROPERTIES AUTOMOC ON)

target_link_libraries(rps_gui
    Qt6::Core
    Qt6::Widgets
)
