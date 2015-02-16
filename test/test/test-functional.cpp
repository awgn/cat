#include <cat/functional.hpp>

#include <string>
#include <iostream>
#include <functional>

#include <yats.hpp>

using namespace yats;
using namespace cat;


Context(callable_test)
{
    int f0(int, std::string, char, bool)
    {
        return 42;
    }

    int negate(int a)
    {
        return -a;
    }

    int add(int a, int  b)
    {
        return a+b;
    }

    Test(basic_callable)
    {
        auto val = callable(negate)(1);
        Assert(val, is_equal_to(-1));
    }


    Test(simple_callable)
    {
        auto add_ = callable(add)(1);
        Assert(add_(2), is_equal_to(3));
    }


    Test(mixed_callable)
    {
        auto f1 = callable(f0)(42, "hello");
        auto f2 = f1('x');

        Assert(f2(true), is_equal_to(42));
    }

    Test(total_callable)
    {
        Assert(callable(f0)(42)("hello")('x')(true), is_equal_to(42));
    }


    void increment(int &n)
    {
        n++;
    }

    Test(lvalue)
    {
        int a = 0;
        callable(increment)(a);
        Assert(a, is_equal_to(1));
    }

    void temporary(int &&n)
    {
        n++;
    }

    Test(rvalue)
    {
        int a = 0;

        callable(temporary)(std::move(a));
        Assert(a, is_equal_to(1));
    }

    Test(apply)
    {
        auto f = callable(f0);
        auto f1 = f.apply(0);
        auto f2 = f1.apply("hello",'x');
        auto f3 = f2.apply(true);

        Assert(callable_traits<decltype(f) >::arity == 4);
        Assert(callable_traits<decltype(f1)>::arity == 3);
        Assert(callable_traits<decltype(f2)>::arity == 1);
        Assert(callable_traits<decltype(f3)>::arity == 0);

        Assert(!std::is_same<decltype(f3), int>::value);

        Assert(f3(), is_equal_to(42));
    }
}


int
main(int argc, char * argv[])
{
    return yats::run(argc, argv);
}


