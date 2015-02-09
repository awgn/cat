cmake_minimum_required(VERSION 2.4)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -std=c++1y")

include_directories(inc)
include_directories(../include)

add_executable(test-functor src/test-functor.cpp)
add_executable(test-applicative src/test-applicative.cpp)
