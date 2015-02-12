#include <cat/functional.hpp>

#include <string>
#include <iostream>
#include <functional>

#include <yats.hpp>

using namespace yats;
using namespace cat;


Context(currying_test)
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

    Test(basic_curry)
    {
        auto val = callable(negate)(1);
        Assert(val, is_equal_to(-1));
    }


    Test(simple_curry)
    {
        auto add_ = callable(add)(1);
        Assert(add_(2), is_equal_to(3));
    }


    Test(mixed_curry)
    {
        auto f1 = callable(f0)(42, "hello");
        auto f2 = f1('x');

        Assert(f2(true), is_equal_to(42));
    }


    Test(full_curry)
    {
        auto add_ = callable(add)(1, 2);
        Assert(add_, is_equal_to(3));
    }


    auto factory_add(int n)
    {
        return callable(add)(n);
    }


    Test(factory_closure)
    {
        auto add2 = factory_add(2);

        Assert(add2(40), is_equal_to(42));
    }

    struct moveable
    {
        moveable()
        : count()
        {}

        moveable(const moveable &) = delete;
        moveable& operator=(const moveable &) = delete;

        moveable(moveable &&other)
        : count(other.count+1)
        {
            other.count = 0;
        }

        moveable&
        operator=(moveable &&other)
        {
            count = other.count+1;
            other.count = 0;
            return *this;
        }

        int count;
    };

}


int
main(int argc, char * argv[])
{
    return yats::run(argc, argv);
}


