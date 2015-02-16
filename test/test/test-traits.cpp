#include <cat/traits.hpp>
#include <cat/utility/type.hpp>
#include <cat/functional.hpp>

#include <yats.hpp>

using namespace yats;
using namespace cat;
using namespace std::placeholders;

Context(traits)
{
    int f0()            { return 0; };
    int f1(int)         { return 0; };
    int f2(int, char)   { return 0; };


    Test(partail_function_type)
    {
       Assert(std::is_same<_partial_function<int(char,short,int,long), 0>::type, int(char,short,int,long)>::value);
       Assert(std::is_same<_partial_function<int(char,short,int,long), 1>::type, int(short,int,long)>::value);
       Assert(std::is_same<_partial_function<int(char,short,int,long), 2>::type, int(int,long)>::value);
       Assert(std::is_same<_partial_function<int(char,short,int,long), 3>::type, int(long)>::value);
       Assert(std::is_same<_partial_function<int(char,short,int,long), 4>::type, int()>::value);
    }


    Test(callable_traits)
    {
        // function

        std::cout << type_of(f0) << std::endl;
        std::cout << type_of(f1) << std::endl;
        std::cout << type_of(f2) << std::endl;

        Assert(std::is_same< callable_traits<decltype(f0)>::type, int()>::value);
        Assert(std::is_same< callable_traits<decltype(f1)>::type, int(int)>::value);
        Assert(std::is_same< callable_traits<decltype(f2)>::type, int(int, char)>::value);

        Assert(callable_traits<decltype(f0)>::arity == 0);
        Assert(callable_traits<decltype(f1)>::arity == 1);
        Assert(callable_traits<decltype(f2)>::arity == 2);

        // function pointer...

        auto p0 = &f0;
        auto p1 = &f1;
        auto p2 = &f2;

        std::cout << type_of(p0) << std::endl;
        std::cout << type_of(p1) << std::endl;
        std::cout << type_of(p2) << std::endl;

        Assert(std::is_same< callable_traits<decltype(p0)>::type, int()>::value);
        Assert(std::is_same< callable_traits<decltype(p1)>::type, int(int)>::value);
        Assert(std::is_same< callable_traits<decltype(p2)>::type, int(int, char)>::value);

        // std::function

        auto fun0 = make_function(f0);
        auto fun1 = make_function(f1);
        auto fun2 = make_function(f2);

        Assert(std::is_same< callable_traits<decltype(fun0)>::type, int()>::value);
        Assert(std::is_same< callable_traits<decltype(fun1)>::type, int(int)>::value);
        Assert(std::is_same< callable_traits<decltype(fun2)>::type, int(int, char)>::value);

        // C++ lambda

        auto l0 = []()          { return 0; };
        auto l1 = [](int)       { return 0; };
        auto l2 = [](int, char) { return 0; };

        std::cout << type_of(l0) << std::endl;
        std::cout << type_of(l1) << std::endl;
        std::cout << type_of(l2) << std::endl;

        Assert(std::is_same< callable_traits<decltype(l0)>::type, int()>::value);
        Assert(std::is_same< callable_traits<decltype(l1)>::type, int(int)>::value);
        Assert(std::is_same< callable_traits<decltype(l2)>::type, int(int, char)>::value);

        // cat::callable

        auto c0 = callable(f0);
        auto c1 = callable(f1);
        auto c2 = callable(f2);
        auto c3 = callable(f2)(42);

        Assert(std::is_same< callable_traits<decltype(c0)>::type, int()>::value);
        Assert(std::is_same< callable_traits<decltype(c1)>::type, int(int)>::value);
        Assert(std::is_same< callable_traits<decltype(c2)>::type, int(int, char)>::value);
        Assert(std::is_same< callable_traits<decltype(c3)>::type, int(char)>::value);
    }
}


int
main(int argc, char * argv[])
{
    return yats::run(argc, argv);
}


