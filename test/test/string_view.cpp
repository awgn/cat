#include <cat/string_view.hpp>
#include <cat/show.hpp>

#include <vector>
#include <list>

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

    .Single("split_on", []
    {
        Assert(cat::split_on("***", "") == std::vector<string_view>{""});
        Assert(cat::split_on("***", "***") == std::vector<string_view>{"", ""});
        Assert(cat::split_on("***", "aaa") == std::vector<string_view>{"aaa"});
        Assert(cat::split_on("***", "***aaa") == std::vector<string_view>{"", "aaa"});
        Assert(cat::split_on("***", "aaa***") == std::vector<string_view>{"aaa", ""});
        Assert(cat::split_on("***", "aaa***bbb") == std::vector<string_view>{"aaa", "bbb"});
        Assert(cat::split_on("***", "aaa******bbb") == std::vector<string_view>{"aaa", "", "bbb"});
        Assert(cat::split_on("***", "aaa*********bbb") == std::vector<string_view>{"aaa", "", "", "bbb"});
    })

    .Single("split_on_no_empty", []
    {
        Assert(cat::split_on("***", "", false) == std::vector<string_view>{});
        Assert(cat::split_on("***", "***", false) == std::vector<string_view>{});
        Assert(cat::split_on("***", "aaa", false) == std::vector<string_view>{"aaa"});
        Assert(cat::split_on("***", "***aaa", false) == std::vector<string_view>{"aaa"});
        Assert(cat::split_on("***", "aaa***", false) == std::vector<string_view>{"aaa"});
        Assert(cat::split_on("***", "aaa***bbb", false) == std::vector<string_view>{"aaa", "bbb"});
        Assert(cat::split_on("***", "aaa******bbb", false) == std::vector<string_view>{"aaa", "bbb"});
        Assert(cat::split_on("***", "aaa*********bbb", false) == std::vector<string_view>{"aaa", "bbb"});
    })

    .Single("split_one_of", []
    {
        Assert(cat::split_one_of("*/+", "") == std::vector<string_view>{""});
        Assert(cat::split_one_of("*/+", "+") == std::vector<string_view>{"", ""});
        Assert(cat::split_one_of("*/+", "aaa") == std::vector<string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "*aaa") == std::vector<string_view>{"", "aaa"});
        Assert(cat::split_one_of("*/+", "**aaa") == std::vector<string_view>{"", "", "aaa"});
        Assert(cat::split_one_of("*/+", "**/aaa") == std::vector<string_view>{"", "", "", "aaa"});
        Assert(cat::split_one_of("*/+", "aaa*") == std::vector<string_view>{"aaa", ""});
        Assert(cat::split_one_of("*/+", "aaa**") == std::vector<string_view>{"aaa", "", ""});
        Assert(cat::split_one_of("*/+", "aaa**+") == std::vector<string_view>{"aaa", "", "", ""});
        Assert(cat::split_one_of("*/+", "aaa*bbb") == std::vector<string_view>{"aaa", "bbb"});
        Assert(cat::split_one_of("*/+", "aaa*+bbb") == std::vector<string_view>{"aaa", "", "bbb"});
        Assert(cat::split_one_of("*/+", "aaa*+/bbb") == std::vector<string_view>{"aaa", "", "", "bbb"});
    })

    .Single("split_one_of_non_empty", []
    {
        Assert(cat::split_one_of("*/+", "", false) == std::vector<string_view>{});
        Assert(cat::split_one_of("*/+", "+", false) == std::vector<string_view>{});
        Assert(cat::split_one_of("*/+", "aaa", false) == std::vector<string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "*aaa", false) == std::vector<string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "**aaa", false) == std::vector<string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "**/aaa", false) == std::vector<string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "aaa*", false) == std::vector<string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "aaa**", false) == std::vector<string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "aaa**+", false) == std::vector<string_view>{"aaa"});
        Assert(cat::split_one_of("*/+", "aaa*bbb", false) == std::vector<string_view>{"aaa", "bbb"});
        Assert(cat::split_one_of("*/+", "aaa*+bbb", false) == std::vector<string_view>{"aaa","bbb"});
        Assert(cat::split_one_of("*/+", "aaa*+/bbb", false) == std::vector<string_view>{"aaa", "bbb"});
    })

    .Single("words", []
    {
        Assert(cat::words("") == std::vector<string_view>{});
        Assert(cat::words(" ") == std::vector<string_view>{});
        Assert(cat::words(" \t") == std::vector<string_view>{});
        Assert(cat::words("aaa") == std::vector<string_view>{"aaa"});
        Assert(cat::words(" aaa") == std::vector<string_view>{"aaa"});
        Assert(cat::words("  \naaa") == std::vector<string_view>{"aaa"});
        Assert(cat::words("aaa ") == std::vector<string_view>{"aaa"});
        Assert(cat::words("aaa \t") == std::vector<string_view>{"aaa"});
        Assert(cat::words("aaa") == std::vector<string_view>{"aaa"});
        Assert(cat::words("aaa\nbbb") == std::vector<string_view>{"aaa", "bbb"});
        Assert(cat::words("aaa \nbbb") == std::vector<string_view>{"aaa","bbb"});
        Assert(cat::words("aaa \n\nbbb") == std::vector<string_view>{"aaa","bbb"});
    })

    .Single("lines", []
    {
        Assert(cat::lines("") == std::vector<string_view>{""});
        Assert(cat::lines("aaa") == std::vector<string_view>{"aaa"});
        Assert(cat::lines("aaa") == std::vector<string_view>{"aaa"});
        Assert(cat::lines("\naaa") == std::vector<string_view>{"", "aaa"});
        Assert(cat::lines("aaa") == std::vector<string_view>{"aaa"});
        Assert(cat::lines("aaa\nbbb") == std::vector<string_view>{"aaa","bbb"});
        Assert(cat::lines("aaa\n\nbbb") == std::vector<string_view>{"aaa", "", "bbb"});
    })

    .Single("intercalate", []
    {
        Assert(cat::intercalate(std::string{","}, std::vector<string_view>{} ) == std::string{""} );
        Assert(cat::intercalate(std::string{","}, std::vector<string_view>{"x"} ) == std::string{"x"} );
        Assert(cat::intercalate(std::string{","}, std::vector<string_view>{"x", "y", "z"} ) == std::string{"x,y,z"} );

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

