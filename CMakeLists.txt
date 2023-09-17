cmake_minimum_required(VERSION 3.26)
project(PelcoD)

set(CMAKE_CXX_STANDARD 23)

add_executable(PelcoD main.cpp
        IPelcoD.h
        PelcoD.cpp
        PelcoD.h)
