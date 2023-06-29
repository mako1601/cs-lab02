cmake_minimum_required(VERSION 3.25.2)

set(PROJECT_NAME Task3)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} main.cpp svg.h histogram.cpp histogram.h)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_include_directories(${PROJECT_NAME} PUBLIC ../external/curl/include)
target_link_libraries(${PROJECT_NAME} PRIVATE "C:\\Users\\yurab\\Desktop\\lab02\\yura\\project\\external\\curl\\lib\\libcurl.dll.a")