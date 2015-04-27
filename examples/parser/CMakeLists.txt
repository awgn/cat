cmake_minimum_required(VERSION 2.4)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -std=c++14")

include_directories(../../)
include_directories(.)

add_executable(key_value-test key_value-test.cpp)
