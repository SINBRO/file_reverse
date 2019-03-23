cmake_minimum_required(VERSION 3.13)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(test types_consts.h file_reader.h file_reader.cpp main.cpp)