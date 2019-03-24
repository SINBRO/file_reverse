cmake_minimum_required(VERSION 3.13)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

add_executable(reverse_file main.cpp file_reverse.cpp file_reverse.h)

add_executable(test test/reverse_file_test.cpp test/gtest-all.cc test/gtest.h test/gtest_main.cc file_reverse.cpp file_reverse.h)

target_link_libraries(test -lpthread)