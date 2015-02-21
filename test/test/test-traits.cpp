#include <cat/bits/typeof.hpp>
#include <cat/type_traits.hpp>
#include <cat/functional.hpp>

#include <yats.hpp>

using namespace yats;
using namespace cat;
using namespace std::placeholders;

Context(traits)
{
    Test(partail_function_type)
    {
       Assert(std::is_same<partial_function_type<int(char,short,int,long), 0>::type, int(char,short,int,long)>::value);
       Assert(std::is_same<partial_function_type<int(char,short,int,long), 1>::type, int(short,int,long)>::value);
       Assert(std::is_same<partial_function_type<int(char,short,int,long), 2>::type, int(int,long)>::value);
       Assert(std::is_same<partial_function_type<int(char,short,int,long), 3>::type, int(long)>::value);
       Assert(std::is_same<partial_function_type<int(char,short,int,long), 4>::type, int()>::value);
    }


    int f0()            { return 0; };
    int f1(int)         { return 0; };
    int f2(int, char)   { return 0; };


    Test(callable_traits)
    {
        // function

        std::cout << type_of(f0) << std::endl;
        std::cout << type_of(f1) << std::endl;
        std::cout << type_of(f2) << std::endl;

        Assert(std::is_same< function_type<decltype(f0)>::type, int()>::value);
        Assert(std::is_same< function_type<decltype(f1)>::type, int(int)>::value);
        Assert(std::is_same< function_type<decltype(f2)>::type, int(int, char)>::value);

        Assert(arity<decltype(f0)>::value == 0);
        Assert(arity<decltype(f1)>::value == 1);
        Assert(arity<decltype(f2)>::value == 2);

        // function pointer...

        auto p0 = &f0;
        auto p1 = &f1;
        auto p2 = &f2;

        std::cout << type_of(p0) << std::endl;
        std::cout << type_of(p1) << std::endl;
        std::cout << type_of(p2) << std::endl;

        Assert(std::is_same< function_type<decltype(p0)>::type, int()>::value);
        Assert(std::is_same< function_type<decltype(p1)>::type, int(int)>::value);
        Assert(std::is_same< function_type<decltype(p2)>::type, int(int, char)>::value);

        // std::function

        auto fun0 = make_function(f0);
        auto fun1 = make_function(f1);
        auto fun2 = make_function(f2);

        Assert(std::is_same< function_type<decltype(fun0)>::type, int()>::value);
        Assert(std::is_same< function_type<decltype(fun1)>::type, int(int)>::value);
        Assert(std::is_same< function_type<decltype(fun2)>::type, int(int, char)>::value);

        // C++ lambda

        auto l0 = []()          { return 0; };
        auto l1 = [](int)       { return 0; };
        auto l2 = [](int, char) { return 0; };

        std::cout << type_of(l0) << std::endl;
        std::cout << type_of(l1) << std::endl;
        std::cout << type_of(l2) << std::endl;

        Assert(std::is_same< function_type<decltype(l0)>::type, int()>::value);
        Assert(std::is_same< function_type<decltype(l1)>::type, int(int)>::value);
        Assert(std::is_same< function_type<decltype(l2)>::type, int(int, char)>::value);

        // cat::callable

        auto c0 = callable(f0);
        auto c1 = callable(f1);
        auto c2 = callable(f2);
        auto c3 = callable(f2)(42);

        Assert(std::is_same< function_type<decltype(c0)>::type, int()>::value);
        Assert(std::is_same< function_type<decltype(c1)>::type, int(int)>::value);
        Assert(std::is_same< function_type<decltype(c2)>::type, int(int, char)>::value);
        Assert(std::is_same< function_type<decltype(c3)>::type, int(char)>::value);
    }

    Test(is_callable)
    {
        Assert(is_callable<int>::value, is_false());

        std::cout << std::is_function<decltype(f0)>::value << std::endl;
        std::cout << std::is_function<decltype(f1)>::value << std::endl;
        std::cout << std::is_function<decltype(f2)>::value << std::endl;

        Assert(is_callable<decltype(f0)>::value, is_true());
        Assert(is_callable<decltype(f1)>::value, is_true());
        Assert(is_callable<decltype(f2)>::value, is_true());

        // function pointer...

        auto p0 = &f0;
        auto p1 = &f1;
        auto p2 = &f2;

        std::cout << type_of((p0)) << std::endl;
        std::cout << type_of((p1)) << std::endl;
        std::cout << type_of((p2)) << std::endl;

        Assert(is_callable<decltype((p0))>::value);
        Assert(is_callable<decltype((p1))>::value);
        Assert(is_callable<decltype((p2))>::value);


        // std::function

        auto fun0 = make_function(f0);
        auto fun1 = make_function(f1);
        auto fun2 = make_function(f2);

        std::cout << type_of(fun0) << std::endl;
        std::cout << type_of(fun1) << std::endl;
        std::cout << type_of(fun2) << std::endl;

        Assert(is_callable<decltype(fun0)>::value);
        Assert(is_callable<decltype(fun1)>::value);
        Assert(is_callable<decltype(fun2)>::value);

        // C++ lambda...

        auto l0 = []()          { return 0; };
        auto l1 = [](int)       { return 0; };
        auto l2 = [](int, char) { return 0; };


        Assert(is_callable< decltype(l0)>::value, is_true());
        Assert(is_callable< decltype(l1)>::value, is_true());
        Assert(is_callable< decltype(l2)>::value, is_true());

        // Generic lambda needs a callable wrapper:
        //

        auto g1 = generic<int(int)>([](auto n) { return n+1; });
        Assert(is_callable< decltype(g1)>::value, is_true());


        // callable...

        auto c0 = callable(f0);
        auto c1 = callable(f1);
        auto c2 = callable(f2);
        auto c3 = callable(f2)(42);

        Assert(is_callable<decltype(c0)>::value, is_true());
        Assert(is_callable<decltype(c1)>::value, is_true());
        Assert(is_callable<decltype(c2)>::value, is_true());
        Assert(is_callable<decltype(c3)>::value, is_true());
    }

    struct Object
    {
        int operator()() const;
        int operator()(int) const;
        int operator()(int, char) const;
    };

    Test(is_callable_with)
    {
        Assert(is_callable_with<int()>::value);
        Assert(is_callable_with<decltype(f1), int>::value);
        Assert(is_callable_with<decltype(f2), int, char>::value);

        Assert(is_callable_as<Object()>::value);
        Assert(is_callable_as<Object(int)>::value);
        Assert(is_callable_as<Object(int,char)>::value);
    }
}


int
main(int argc, char * argv[])
{
    return yats::run(argc, argv);
}


