#include <cat/infix.hpp>
#include <cat/bits/constexpr.hpp>

#include <type_traits>

#include "yats.hpp"

using namespace yats;
using namespace cat;


// Tests:
//

Context(test_infix)
{

    struct sum
    {
        template <typename T>
        constexpr auto operator()(T a, T b) const
        {
            return a + b;
        }
    };

    Test(simple)
    {
        constexpr auto s = infix_adaptor<sum>{};

        Assert ( 1 *s* 2 == 3 );
        Assert ( 1 /s/ 2 == 3 );
        Assert ( 1 %s% 2 == 3 );

        Assert ( 1 +s+ 2 == 3 );
        Assert ( 1 -s- 2 == 3 );

        Assert ( (1 <s> 2) == 3 );
        Assert ( (1 &s& 2) == 3 );
        Assert ( (1 ^s^ 2) == 3 );
        Assert ( (1 |s| 2) == 3 );
    }

    int sum_inc_(int &a, int &b)
    {
        return ++a + ++b;
    }

    constexpr auto sum_inc = infix(sum_inc_);

    Test(lvalue)
    {
        int a = 2, b = 3;

        auto c = a /sum_inc/ b;

        Assert(a, is_equal_to(3));
        Assert(b, is_equal_to(4));
        Assert(c, is_equal_to(7));

        auto d = a *sum_inc* b;

        Assert(a, is_equal_to(4));
        Assert(b, is_equal_to(5));
        Assert(d, is_equal_to(9));
    }

    int sum_tmp_(int &&a, int &&b)
    {
        return a + b;
    }

    constexpr auto sum_tmp = infix(sum_tmp_);

    Test(rvalue)
    {
        int a = 2, b = 3;

        auto c = std::move(a) %sum_tmp% std::move(b);

        Assert(a, is_equal_to(2));
        Assert(b, is_equal_to(3));
        Assert(c, is_equal_to(5));
    }

    struct plus_
    {
        template <typename T>
        constexpr auto operator()(T a, T b) const
        {
            return a+b;
        }
    };

    Test(constexpr)
    {
        constexpr int val = 0;
        constexpr auto plus = infix_adaptor<plus_>{};

        assert_constexpr(val *plus* val);
        assert_constexpr(val /plus/ val);
        assert_constexpr(val %plus% val);
        assert_constexpr(val +plus+ val);
        assert_constexpr(val -plus- val);
        assert_constexpr(val <plus> val);
        assert_constexpr(val &plus& val);
        assert_constexpr(val ^plus^ val);
        assert_constexpr(val |plus| val);
    }

}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

