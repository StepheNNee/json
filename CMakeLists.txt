cmake_minimum_required(VERSION 3.23)
project(json)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(json
        generator.cpp
        generator.h
        parser.cpp
        parser.h
        test.cpp
        value.cpp
        value.h)
