cmake_minimum_required(VERSION 2.4)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -std=c++1y")

include_directories(inc)
include_directories(../.)

add_executable(test-infix test/test-infix.cpp)
add_executable(test-tuple test/test-tuple.cpp)
add_executable(test-traits test/test-traits.cpp)
add_executable(test-functional test/test-functional.cpp)
add_executable(test-iterator test/test-iterator.cpp)
add_executable(test-forward test/test-forward.cpp)

add_executable(test-monad test/test-monad.cpp)
add_executable(test-functor test/test-functor.cpp)
add_executable(test-applicative test/test-applicative.cpp)
add_executable(test-monoid test/test-monoid.cpp)
add_executable(test-bifunctor test/test-bifunctor.cpp)
