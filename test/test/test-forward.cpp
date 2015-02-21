#include <cat/utility.hpp>
#include <cat/iterator.hpp>
#include <cat/bits/type.hpp>

#include <vector>
#include <iterator>


using namespace cat;


template <typename T>
void function_impl(T && x)
{
    for(auto && elem : x)
    {
        std::cout << type_of(forward_as<T>(elem)) << std::endl;
    }

    auto it = auto_begin(std::forward<T>(x));
    for(; it != auto_end(std::forward<T>(x)); ++it)
    {
        std::cout << type_of(*it) << std::endl;
    }
}


void function(std::vector<int> && x)
{
     function_impl(std::move(x));
}

void function(std::vector<int> const &x)
{
     function_impl(x);
}


int
main(int, char *[])
{
    std::vector<int> v {1,2,3};

    std::cout << "LVALUE:" << std::endl;
    function(v);

    std::cout << "RVALUE:" << std::endl;
    function(std::move(v));

    return 0;
}

