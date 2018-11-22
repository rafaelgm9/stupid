cmake_minimum_required(VERSION 3.12)
project(stupid)

set(CMAKE_CXX_STANDARD 17)


file(GLOB source src/*.cpp src/*.h)

add_library(stupid ${source})

target_include_directories(stupid PUBLIC include)


add_executable(test test/test.cpp)

target_link_libraries(test stupid gtest gtest_main pthread)

target_include_directories(test PUBLIC include src)