#include <cat/curry.hpp>

#include <string>
#include <iostream>
#include <functional>

#include <yats.hpp>

using namespace yats;
using namespace cat;


Context(currying_test)
{
    void f0(int a, std::string b, char c, bool q)
    {
        std::cout << std::boolalpha << a << ' ' << b << ' ' << c << ' ' << q << '!' << std::endl;
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
        auto val = curry(negate,1);
        Assert(val(), is_equal_to(-1));
    }


    Test(simple_curry)
    {
        auto add_ = curry(add,1);
        Assert(add_(2), is_equal_to(3));
    }


    Test(mixed_curry)
    {
        auto f1 = curry(f0, 42, "hello");
        auto f2 = curry(f1, 'x');
        f2(true);
    }


    Test(full_curry)
    {
        auto add_ = curry(add, 1, 2);
        Assert(add_(), is_equal_to(3));
    }


    Test(basic_closure)
    {
        auto val = closure(negate,1);
        Assert(val(), is_equal_to(-1));
    }


    Test(simple_closure)
    {
        auto add_ = closure(add,1);
        Assert(add_(2), is_equal_to(3));
    }


    Test(mixed_closure)
    {
        auto f1 = closure(f0, 42, std::string("hello"));
        auto f2 = closure(f1, 'x');
        f2(true);
    }

    Test(full_closure)
    {
        auto add_ = closure(add, 1, 2);
        Assert(add_(), is_equal_to(3));
    }


    std::function<int(int)> factory_add(int n)
    {
        return closure(add,n);
    }


    Test(factory_closure)
    {
        auto add2 = factory_add(2);

        Assert(add2(40), is_equal_to(42));
    }

    void incr(int &n)
    {
        n++;
    }

    Test(lval_ref_curry)
    {
        int  n = 0;
        auto f = curry(incr, n);
        f();
        Assert(n, is_equal_to(1));
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

    int take(moveable &&m)
    {
        return m.count;
    }

    Test(moveable_curry)
    {
        moveable m;
        auto z = curry(&take, std::move(m));
        auto x = z();

        Assert(x, is_equal_to(1));
    }
}


int
main(int argc, char * argv[])
{
    return yats::run(argc, argv);
}


