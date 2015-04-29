#include <cat/functional.hpp>
#include <cat/placeholders.hpp>
#include <cat/bits/constexpr.hpp>

#include <algorithm>
#include <string>
#include <iostream>
#include <functional>
#include <type_traits>

#include <yats.hpp>

using namespace yats;
using namespace cat;
using namespace cat::placeholders;

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

    Test(basic_currying)
    {
        auto val = currying(negate)(1);
        Assert(val, is_equal_to(-1));
    }


    Test(simple_currying)
    {
        auto add_ = currying(add)(1);
        Assert(add_(2), is_equal_to(3));
    }


    Test(mixed_currying)
    {
        auto f1 = currying(f0)(42, "hello");
        auto f2 = f1('x');

        Assert(f2(true), is_equal_to(42));
    }

    Test(total_currying)
    {
        Assert(currying(f0)(42)("hello")('x')(true), is_equal_to(42));
    }


    Test(template_currying)
    {
        int n = 1;

        auto f = generic<_a(_a,_a)>([](auto x, auto y) { return (x++) + y;});

        Assert(f(n)(2), is_equal_to(3));
        Assert(n, is_equal_to(1));

        auto g = generic<_a(_a &,_a)>([](auto & x, auto y) { return (x++) + y;});

        Assert(g(n)(2), is_equal_to(3));
        Assert(n, is_equal_to(2));

    }

    void increment(int &n)
    {
        n++;
    }

    Test(lvalue)
    {
        int a = 0;
        currying(increment)(a);
        Assert(a, is_equal_to(1));
    }

    void temporary(int &&n)
    {
        n++;
    }

    Test(rvalue)
    {
        int a = 0;

        currying(temporary)(std::move(a));
        Assert(a, is_equal_to(1));
    }

    Test(apply)
    {
        auto f = currying(f0);
        auto f1 = f.apply(0);
        auto f2 = f1.apply("hello",'x');
        auto f3 = f2.apply(true);

        Assert(function_arity<decltype(f) >::value == 4);
        Assert(function_arity<decltype(f1)>::value == 3);
        Assert(function_arity<decltype(f2)>::value == 1);
        Assert(function_arity<decltype(f3)>::value == 0);

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
        assert_constexpr(currying(identity));
        assert_constexpr(generic<int(int)>(identity));
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
        return a + b;
    }

    Test(composition)
    {
        auto h1 = compose(sum,next);
        auto h2 = compose(currying(sum),next);
        auto h3 = compose(sum,currying(next));
        auto h4 = compose(currying(sum),currying(next));

        Assert(h1(10,1)   == 12);
        Assert(h2(10)(1)  == 12);
        Assert(h3(10,1)   == 12);
        Assert(h4(10)(1)  == 12);

        auto l1 = compose(currying(sum), [](int n) { return n+1; });
        auto l2 = l1(10);

        Assert( l2(1) == 12);

        auto x1 = compose(sum, constant);
        Assert(x1(1) == 43);

        auto x2 = currying(sum) ^ (constant);
        Assert(x2(1) == 43);
    }


    Test(composition2)
    {
        auto h1 = compose(flip(sum), next);
        auto h2 = compose(h1, next);

        Assert(h1(2,3), is_equal_to(6));
        Assert(h2(2,3), is_equal_to(7));
    }


    Test(generic)
    {
        auto h1 = generic<int(int, int)>([](auto a, auto b) { return a+b;});

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

        std::sort(std::begin(v), std::end(v), std::less<int>() |on| first);
        std::sort(std::begin(v), std::end(v), currying(generic<bool(int,int)>(std::less<int>()) |on| first));

        Assert(first(v[0]) == 1);
        Assert(first(v[1]) == 2);

#if defined(__clang__) || (__GNUC__ >= 5)
        std::sort(std::begin(v), std::end(v), std::less<std::string>{} -on- elem<1>);
        Assert(first(v[0]) == 2);
        Assert(first(v[1]) == 1);
#endif

    }

    Test(currying_noncopyable)
    {
        auto p1 = std::make_unique<int>(42);
        auto p2 = std::make_unique<int>(42);

        auto f0 = [](std::unique_ptr<int> &&)      { return 0; };
        auto f1 = [](std::unique_ptr<int> const &) { return 0; };
        auto f2 = [](std::unique_ptr<int>)         { return 0; };

        f0(std::move(p1));
        f1(p1);
        f2(std::move(p1));

        currying(f0)(std::move(p2));
        currying(f1)(p2);
        currying(f2)(std::move(p2));
    }

    Test(currying_advanced)
    {
        auto f1 = [](int n)        { return n; };
        auto f2 = [](int &n)       { n+=1; return n; };
        auto f3 = [](int const &n) { return n; };
        auto f4 = [](int &&n)      { n+=2; return n; };

        int a = 1, b = 1;

        Assert( f1(a) == currying(f1)(b) );
        Assert( f2(a) == currying(f2)(b) );
        Assert( f3(a) == currying(f3)(b) );

        Assert(a, is_equal_to(b));

        Assert( f1(std::move(a)) == currying(f1)(std::move(b)) );
        Assert( f3(std::move(a)) == currying(f3)(std::move(b)) );
        Assert( f4(std::move(a)) == currying(f4)(std::move(b)) );

        Assert(a, is_equal_to(b));

        // non copyable...
        //

        auto g1 = [](std::unique_ptr<int> const &n) { return *n; };
        auto g2 = [](std::unique_ptr<int> &n)       { (*n)++; return *n; };
        auto g3 = [](std::unique_ptr<int> && n)     { (*n)++; return *n; };
        auto g4 = [](std::unique_ptr<int> n)        { return *n; };

        auto x = std::make_unique<int>(42);
        auto y = std::make_unique<int>(42);

        Assert (g1(x)  == currying(g1)(y));
        Assert (g2(x)  == currying(g2)(y));
        Assert (g3(std::move(x)) == currying(g3)(std::move(y)));

        Assert (*x == *y);

        Assert (g4(std::move(x)) == currying(g4)(std::move(y)));
        Assert(!x and !y);
    }

}


int
main(int argc, char * argv[])
{
    return yats::run(argc, argv);
}


