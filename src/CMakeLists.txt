cmake_minimum_required(VERSION 3.25.2)

add_executable(Task3 main.cpp svg.h histogram.cpp histogram.h)

target_compile_features(Task3 PUBLIC cxx_std_17)

target_include_directories(Task3 PUBLIC ../external/curl/include)
target_link_libraries(Task3 ../external/curl/lib/libcurl.a)

install(TARGETS Task3)