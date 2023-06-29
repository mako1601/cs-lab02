cmake_minimum_required(VERSION 3.25.2)

add_executable(Test test.cpp histogram.cpp histogram.h)

target_compile_features(Test PUBLIC cxx_std_17)
