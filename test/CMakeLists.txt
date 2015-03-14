cmake_minimum_required(VERSION 2.4)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -std=c++14")

include_directories(inc)
include_directories(../.)

add_executable(test-container test/container.cpp)
add_executable(test-traits test/traits.cpp)
add_executable(test-infix test/infix.cpp)
add_executable(test-tuple test/tuple.cpp)
add_executable(test-functional test/functional.cpp)
add_executable(test-iterator test/iterator.cpp)
add_executable(test-optional test/optional.cpp)
add_executable(test-forward test/forward.cpp)
add_executable(test-string_view test/string_view.cpp)

add_executable(test-functor test/functor.cpp)
add_executable(test-bifunctor test/bifunctor.cpp)
add_executable(test-monoid test/monoid.cpp)
add_executable(test-applicative test/applicative.cpp)
add_executable(test-monad test/monad.cpp)
add_executable(test-foldable test/foldable.cpp)

add_executable(test-show test/show.cpp)
add_executable(test-read test/read.cpp)

target_link_libraries(test-functor -pthread)
target_link_libraries(test-applicative -pthread)
target_link_libraries(test-monad -pthread)
target_link_libraries(test-monoid -pthread)
