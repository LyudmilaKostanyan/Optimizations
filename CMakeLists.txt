cmake_minimum_required(VERSION 3.10)
project(optimization)
set(CMAKE_CXX_STANDARD 23)

if(NOT DEFINED OPT_LEVEL)
    set(OPT_LEVEL O0)
endif()

message(STATUS "Using compiler optimization: -${OPT_LEVEL}")
set(CMAKE_CXX_FLAGS "-${OPT_LEVEL}")

add_executable(main main.cpp)
