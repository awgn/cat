#include <cat/string_view.hpp>

#include "yats.hpp"

using namespace yats;
using namespace cat;

// Tests:
//

Context(test_read)
{
    Test (consume)
    {
        string_view spaces("    ");
        string_view s(" hello world ");

        Assert( cat::consume('x', spaces) == std::experimental::nullopt);
        Assert( cat::consume('x', s) == std::experimental::nullopt);
        Assert( cat::consume('h', s).value() == string_view{"ello world "});

        Assert( cat::consume("ciao", spaces) == std::experimental::nullopt);
        Assert( cat::consume("ciao", s) == std::experimental::nullopt);
        Assert( cat::consume("hello", s).value() == string_view{" world "});
    }

    Test(trim)
    {
        string_view spaces("    ");
        string_view s0(" hello world ");
        string_view s1(" hello");
        string_view s2("world!   ");

        Assert(cat::trim(spaces) == string_view{});
        Assert(cat::trim(s0) == string_view{"hello world"});
        Assert(cat::trim(s1) == string_view{"hello"});
        Assert(cat::trim(s2) == string_view{"world!"});
    }
}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

