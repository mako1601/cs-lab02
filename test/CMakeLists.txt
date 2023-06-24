cmake_minimum_required(VERSION 3.25.2)

add_executable(Test test.cpp ../src/histogram.cpp ../src/histogram.h)

set(CMAKE_CXX_STANDARD 17)