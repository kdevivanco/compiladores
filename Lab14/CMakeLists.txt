cmake_minimum_required(VERSION 3.25)
project(Lab14)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Lab14
        environment.h
        exp.cpp
        exp.h
        main.cpp
        parser.cpp
        parser.h
        scanner.cpp
        scanner.h
        token.cpp
        token.h
        visitor.cpp
        visitor.h)
