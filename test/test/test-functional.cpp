#include <cat/functional.hpp>
#include <cat/utility/constexpr.hpp>

#include <algorithm>
#include <string>
#include <iostream>
#include <functional>
#include <type_traits>

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

        Assert(arity<decltype(f) >::value == 4);
        Assert(arity<decltype(f1)>::value == 3);
        Assert(arity<decltype(f2)>::value == 1);
        Assert(arity<decltype(f3)>::value == 0);

        Assert(!std::is_same<decltype(f3), int>::value);

        Assert(f3(), is_equal_to(42));
    }

    struct Diff
    {
        constexpr Diff() {}
        int operator()(int a, int b) const
        {
            return a - b;
        }
    };

    Test(constexpr)
    {
        constexpr auto diff = Diff{};

        assert_constexpr(identity);
        assert_constexpr(callable(identity));
        assert_constexpr(generic<1>(identity));
        assert_constexpr(compose(identity, identity));
        assert_constexpr(flip(diff));
    }


    int next(int a)
    {
        return a+1;
    }

    int constant() { return 42; }

    int sum(int a, int b)
    {
        return a - b;
    }

    Test(composition)
    {
        auto h1 = compose(sum,next);
        auto h2 = compose(callable(sum),next);
        auto h3 = compose(sum,callable(next));
        auto h4 = compose(callable(sum),callable(next));

        Assert(h1(10,1)  == 10);
        Assert(h2(10)(1) == 10);
        Assert(h3(10,1)  == 10);
        Assert(h4(10)(1)  == 10);

        auto l1 = compose(callable(sum), [](int n) { return n+1; });
        auto l2 = l1(10);

        Assert( l2(1) == 10);

        auto x1 = compose(sum, constant);
        Assert(x1(1) == 41);

        auto x2 = callable(sum) ^ (constant);
        Assert(x2(1) == 41);
    }


    Test(generic)
    {
        auto h1 = callable(generic<2>([](auto a, auto b) { return a+b;}));

        Assert(h1(1,2) == 3);
        Assert(h1(1)(2) == 3);
    }

    struct Diff_
    {
        int operator()(int a, int b) const
        {
            return a - b;
        }
    };

    constexpr auto diff = Diff_{};

    Test(flip)
    {
        constexpr auto rev = flip(diff);

        assert_constexpr(rev);
        Assert (rev(1,2) == 1);
    }

    Test(tuple)
    {
        std::pair<int, int> x { 1, 1};

        first(x) = 2;
        second(x) = 3;

        Assert( x == std::make_pair(2, 3) );
    }


    Test(on)
    {
        std::vector<std::pair<int, std::string>> v { {2, "abc"}, {1, "hello"} };

        // std::sort(std::begin(v), std::end(v), on(std::less<int>(), first)); or better...
        //

        std::sort(std::begin(v), std::end(v), std::less<int>() |on| first);

        Assert(first(v[0]) == 1);
        Assert(first(v[1]) == 2);

        std::sort(std::begin(v), std::end(v), std::less<std::string>{} -on- elem<1>);

        Assert(first(v[0]) == 2);
        Assert(first(v[1]) == 1);
    }

}


int
main(int argc, char * argv[])
{
    return yats::run(argc, argv);
}


