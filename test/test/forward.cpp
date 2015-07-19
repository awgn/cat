#include <cat/utility.hpp>
#include <cat/iterator.hpp>
#include <cat/bits/type.hpp>

#include <vector>
#include <iterator>

#include <yats.hpp>

using namespace cat;
using namespace yats;


template <typename T>
void assert_lvalue(T && x)
{
    std::cout << "L-VALUE (" << type_name<T>() << "):" << std::endl;

    std::cout << "forward_as<T>:" << std::endl;

    for(auto && elem : x)
    {
        std::cout << type_of(forward_as<T>(elem)) << std::endl;

        Assert(std::is_lvalue_reference<decltype(forward_as<T>(elem))>::value);
    }

    std::cout << "forward_iterator<T>:" << std::endl;

    auto it = forward_iterator<T>(std::begin(x));
    for(; it != forward_iterator<T>(std::end(x)); ++it)
    {
        std::cout << type_of(*it) << std::endl;

        Assert(std::is_lvalue_reference<decltype(*it)>::value);
    }
}


template <typename T>
void assert_rvalue(T && x)
{
    std::cout << "R-VALUE (" << type_name<T>() << "):" << std::endl;

    std::cout << "forward_as<T>:" << std::endl;

    for(auto && elem : x)
    {
        std::cout << type_of(forward_as<T>(elem)) << std::endl;

        Assert(std::is_rvalue_reference<decltype(forward_as<T>(elem))>::value);
    }

    std::cout << "forward_iterator<T>:" << std::endl;

    auto it = forward_iterator<T>(std::begin(x));
    for(; it != forward_iterator<T>(std::end(x)); ++it)
    {
        std::cout << type_of(*it) << std::endl;

        Assert(std::is_rvalue_reference<decltype(*it)>::value);
    }
}


auto g = Group("test_forward")

    .Single("lvalue", []
    {
        std::vector<int> v{42};
        assert_lvalue(v);

        std::vector<int> const c { 11 };
        assert_lvalue(c);
    })

    .Single("rvalue", []
    {
        std::vector<int> v{42};
        assert_rvalue(std::move(v));
    });

int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

