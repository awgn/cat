#include <cat/string_view.hpp>
#include <cat/show.hpp>

#include <vector>
#include <list>
#include <string_view>
#include <cat/string_view.hpp>
#include "yats.hpp"

using namespace yats;
using namespace cat;

// Tests:
//

auto g = Group("test_read")

    .Single ("consume", []
    {
        std::string_view spaces("    ");
        std::string_view s(" hello world ");

        Assert( cat::consume_char('x', spaces) == std::nullopt);
        Assert( cat::consume_char('x', s) == std::nullopt);
        Assert( cat::consume_char('h', s).value() == std::string_view{"ello world "});

        Assert( cat::consume_string("ciao", spaces) == std::nullopt);
        Assert( cat::consume_string("ciao", s) == std::nullopt);
        Assert( cat::consume_string("hello", s).value() == std::string_view{" world "});
    })

    .Single("trim", []
    {
        Assert(cat::trim("") == std::string_view{});
        Assert(cat::trim("   ") == std::string_view{});
        Assert(cat::trim("hello world") == std::string_view{"hello world"});
        Assert(cat::trim("    hello world") == std::string_view{"hello world"});
        Assert(cat::trim("hello world   ") == std::string_view{"hello world"});
        Assert(cat::trim("   hello world   ") == std::string_view{"hello world"});
    })

    .Single("split_on", []
    {
        Assert(cat::split_on("***", "") == std::vector<std::string_view>{""});
        Assert(cat::split_on("***", "***") == std::vector<std::string_view>{"", ""});
        Assert(cat::split_on("***", "aaa") == std::vector<std::string_view>{"aaa"});
        Assert(cat::split_on("***", "***aaa") == std::vector<std::string_view>{"", "aaa"});
        Assert(cat::split_on("***", "aaa***") == std::vector<std::string_view>{"aaa", ""});
        Assert(cat::split_on("***", "aaa***bbb") == std::vector<std::string_view>{"aaa", "bbb"});
        Assert(cat::split_on("***", "aaa******bbb") == std::vector<std::string_view>{"aaa", "", "bbb"});
        Assert(cat::split_on("***", "aaa*********bbb") == std::vector<std::string_view>{"aaa", "", "", "bbb"});
    })

    .Single("split_on_no_empty", []
    {
        Assert(cat::split_on("***", "", false) == std::vector<std::string_view>{});
        Assert(cat::split_on("***", "***", false) == std::vector<std::string_view>{});
        Assert(cat::split_on("***", "aaa", false) == std::vector<std::string_view>{"aaa"});
        Assert(cat::split_on("***", "***aaa", false) == std::vector<std::string_view>{"aaa"});
        Assert(cat::split_on("***", "aaa***", false) == std::vector<std::string_view>{"aaa"});
        Assert(cat::split_on("***", "aaa***bbb", false) == std::vector<std::string_view>{"aaa", "bbb"});
        Assert(cat::split_on("***", "aaa******bbb", false) == std::vector<std::string_view>{"aaa", "bbb"});
        Assert(cat::split_on("***", "aaa*********bbb", false) == std::vector<std::string_view>{"aaa", "bbb"});
    })

    .Single("split_one_of", []
    {
        Assert(cat::split_one_of("*/+", "") == std::vector<std::string_view>{""});
        Assert(cat::split_one_of("*/+", "+") == std::vector<std::string_view>{"", ""});
        Assert(cat::split_one_of("*/+", "aaa") == std::vector<std::string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "*aaa") == std::vector<std::string_view>{"", "aaa"});
        Assert(cat::split_one_of("*/+", "**aaa") == std::vector<std::string_view>{"", "", "aaa"});
        Assert(cat::split_one_of("*/+", "**/aaa") == std::vector<std::string_view>{"", "", "", "aaa"});
        Assert(cat::split_one_of("*/+", "aaa*") == std::vector<std::string_view>{"aaa", ""});
        Assert(cat::split_one_of("*/+", "aaa**") == std::vector<std::string_view>{"aaa", "", ""});
        Assert(cat::split_one_of("*/+", "aaa**+") == std::vector<std::string_view>{"aaa", "", "", ""});
        Assert(cat::split_one_of("*/+", "aaa*bbb") == std::vector<std::string_view>{"aaa", "bbb"});
        Assert(cat::split_one_of("*/+", "aaa*+bbb") == std::vector<std::string_view>{"aaa", "", "bbb"});
        Assert(cat::split_one_of("*/+", "aaa*+/bbb") == std::vector<std::string_view>{"aaa", "", "", "bbb"});
    })

    .Single("split_one_of_non_empty", []
    {
        Assert(cat::split_one_of("*/+", "", false) == std::vector<std::string_view>{});
        Assert(cat::split_one_of("*/+", "+", false) == std::vector<std::string_view>{});
        Assert(cat::split_one_of("*/+", "aaa", false) == std::vector<std::string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "*aaa", false) == std::vector<std::string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "**aaa", false) == std::vector<std::string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "**/aaa", false) == std::vector<std::string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "aaa*", false) == std::vector<std::string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "aaa**", false) == std::vector<std::string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "aaa**+", false) == std::vector<std::string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "aaa*bbb", false) == std::vector<std::string_view>{"aaa", "bbb"});
        Assert(cat::split_one_of("*/+", "aaa*+bbb", false) == std::vector<std::string_view>{"aaa","bbb"});
        Assert(cat::split_one_of("*/+", "aaa*+/bbb", false) == std::vector<std::string_view>{"aaa", "bbb"});
    })

    .Single("words", []
    {
        Assert(cat::words("") == std::vector<std::string_view>{});
        Assert(cat::words(" ") == std::vector<std::string_view>{});
        Assert(cat::words(" \t") == std::vector<std::string_view>{});
        Assert(cat::words("aaa") == std::vector<std::string_view>{"aaa"});
        Assert(cat::words(" aaa") == std::vector<std::string_view>{"aaa"});
        Assert(cat::words("  \naaa") == std::vector<std::string_view>{"aaa"});
        Assert(cat::words("aaa ") == std::vector<std::string_view>{"aaa"});
        Assert(cat::words("aaa \t") == std::vector<std::string_view>{"aaa"});
        Assert(cat::words("aaa") == std::vector<std::string_view>{"aaa"});
        Assert(cat::words("aaa\nbbb") == std::vector<std::string_view>{"aaa", "bbb"});
        Assert(cat::words("aaa \nbbb") == std::vector<std::string_view>{"aaa","bbb"});
        Assert(cat::words("aaa \n\nbbb") == std::vector<std::string_view>{"aaa","bbb"});
    })

    .Single("lines", []
    {
        Assert(cat::lines("") == std::vector<std::string_view>{""});
        Assert(cat::lines("aaa") == std::vector<std::string_view>{"aaa"});
        Assert(cat::lines("aaa") == std::vector<std::string_view>{"aaa"});
        Assert(cat::lines("\naaa") == std::vector<std::string_view>{"", "aaa"});
        Assert(cat::lines("aaa") == std::vector<std::string_view>{"aaa"});
        Assert(cat::lines("aaa\nbbb") == std::vector<std::string_view>{"aaa","bbb"});
        Assert(cat::lines("aaa\n\nbbb") == std::vector<std::string_view>{"aaa", "", "bbb"});
    })

    .Single("intercalate", []
    {
        Assert(cat::intercalate(std::string{","}, std::vector<std::string_view>{} ) == std::string{""} );
        Assert(cat::intercalate(std::string{","}, std::vector<std::string_view>{"x"} ) == std::string{"x"} );
        Assert(cat::intercalate(std::string{","}, std::vector<std::string_view>{"x", "y", "z"} ) == std::string{"x,y,z"} );

        Assert(cat::intercalate(std::string{","}, std::list<std::string>{} ) == std::string{""} );
        Assert(cat::intercalate(std::string{","}, std::list<std::string>{"x"} ) == std::string{"x"} );
        Assert(cat::intercalate(std::string{","}, std::list<std::string>{"x", "y", "z"} ) == std::string{"x,y,z"} );
    })
    ;

int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}
