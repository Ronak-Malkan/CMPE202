cmake_minimum_required(VERSION 3.10)
project(RockPaperScissors)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable
add_executable(rps src/main.cpp)

# Include directories
target_include_directories(rps PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
