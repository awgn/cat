#include <cat/utility/infix.hpp>

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
        auto operator()(T a, T b) const
        {
            return a + b;
        }
    };

    Test(simple)
    {
        auto s = infix_adaptor<sum>{};

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

    int sum_inc(int &a, int &b)
    {
        return ++a + ++b;
    }

    auto fun = infix(sum_inc);

    Test(lvalue)
    {
        int a = 2, b = 3;

        auto c = a /fun/ b;

        Assert(a, is_equal_to(3));
        Assert(b, is_equal_to(4));
        Assert(c, is_equal_to(7));

        auto d = a *fun* b;

        Assert(a, is_equal_to(4));
        Assert(b, is_equal_to(5));
        Assert(d, is_equal_to(9));
    }

    struct oper_t
    {
        constexpr oper_t() { }
    };

    template <typename T>
    constexpr bool check_constexpr(T)
    {
        return true;
    }

    constexpr auto oper = infix(oper_t{});

    Test(constexpr)
    {
        std::integral_constant<bool, check_constexpr(oper)> ok;
        (void) ok;
    }

}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

