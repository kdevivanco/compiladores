cmake_minimum_required(VERSION 3.25)
project(untitled1)

set(CMAKE_CXX_STANDARD 23)

add_executable(untitled1
        main.cpp
        parser.cpp
        scanner.cpp
        token.cpp
        visitor.cpp
        exp.cpp
        environment.h
        exp.h
        parser.h
        scanner.h
        token.h
        visitor.h)

