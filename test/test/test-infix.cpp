#include <cat/utility/infix.hpp>

#include "yats.hpp"

using namespace yats;
using namespace cat;


// Tests:
//

Context(infix)
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

    int odd(int &a, int &b)
    {
        return ++a + ++b;
    }

    Test(lvalue)
    {
        int a = 2, b = 3;

        auto fun = make_infix(odd);
        auto c = a /fun/ b;

        Assert(a, is_equal_to(3));
        Assert(b, is_equal_to(4));
        Assert(c, is_equal_to(7));
    }

}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

