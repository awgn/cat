#include <iostream>
#include <cat/match.hpp>

using namespace cat;

template <typename T>
void fun(T && value)
{
    case_of(std::forward<T>(value))
        .match
        (
            [](int & n)         { std::cout << "L-value int = " << n << std::endl; },
            [](int && n)        { std::cout << "R-value int = " << n << std::endl; },
            [](char c)          { std::cout << "char = "        << c << std::endl; },
            [](std::string s)   { std::cout << "string = "      << s << std::endl; }
        );
}

int
main(int, char *[])
{
    int x = 13;

    fun('c');
    fun(x);
    fun(42);
    fun(std::string("hello world"));

    return 0;
}

