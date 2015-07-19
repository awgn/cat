#include <cat/string_view.hpp>
#include <vector>

#include "yats.hpp"

using namespace yats;
using namespace cat;

// Tests:
//

auto g = Group("test_read")

    .Single ("consume", []
    {
        string_view spaces("    ");
        string_view s(" hello world ");

        Assert( cat::consume_char('x', spaces) == nullopt);
        Assert( cat::consume_char('x', s) == nullopt);
        Assert( cat::consume_char('h', s).value() == string_view{"ello world "});

        Assert( cat::consume_string("ciao", spaces) == nullopt);
        Assert( cat::consume_string("ciao", s) == nullopt);
        Assert( cat::consume_string("hello", s).value() == string_view{" world "});
    })

    .Single("trim", []
    {
        Assert(cat::trim("") == string_view{});
        Assert(cat::trim("   ") == string_view{});
        Assert(cat::trim("hello world") == string_view{"hello world"});
        Assert(cat::trim("    hello world") == string_view{"hello world"});
        Assert(cat::trim("hello world   ") == string_view{"hello world"});
        Assert(cat::trim("   hello world   ") == string_view{"hello world"});
    })

    .Single("split", []
    {
        Assert(cat::split("", "***") == std::vector<std::string>{""});
        Assert(cat::split("***", "***") == std::vector<std::string>{"", ""});
        Assert(cat::split("aaa", "***") == std::vector<std::string>{"aaa"});
        Assert(cat::split("***aaa", "***") == std::vector<std::string>{"", "aaa"});
        Assert(cat::split("aaa***", "***") == std::vector<std::string>{"aaa", ""});
        Assert(cat::split("aaa***bbb", "***") == std::vector<std::string>{"aaa", "bbb"});
    });


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

