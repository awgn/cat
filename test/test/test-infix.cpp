#include <cat/infix.hpp>

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

        Assert ( 1 <s> 2 == 3 );

        Assert ( (1 &s& 2) == 3 );
        Assert ( (1 ^s^ 2) == 3 );
        Assert ( (1 |s| 2) == 3 );
    }
}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

