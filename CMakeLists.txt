cmake_minimum_required(VERSION 3.5)

project(Decoder)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_MAKE_PROGRAM make)

add_executable(${PROJECT_NAME} main.cpp)
