#include <cat/read.hpp>
#include <cat/bits/type.hpp>

#include <utility>
#include "yats.hpp"

using namespace yats;
using namespace cat;

// Tests:
//

Context(test_read)
{
    Test(simple)
    {
        Assert(read_opt<short>("...") == std::experimental::nullopt);

        Assert(read_opt<unsigned char>(" test").value() == std::make_pair<unsigned char, string_view>('t', "est"));

        Assert(read_opt<short>("123").value() == std::make_pair<short, string_view>(123, ""));
        Assert(read_opt<int>("123").value() == std::make_pair<int, string_view>(123, ""));

        Assert(read_opt<long>("-123").value() == std::make_pair<long, string_view>(-123, ""));
        Assert(read_opt<long long>("-123").value() == std::make_pair<long long, string_view>(-123, ""));

        Assert(read_opt<unsigned short>("42").value() == std::make_pair<unsigned short, string_view>(42, ""));
        Assert(read_opt<unsigned>("42").value() == std::make_pair<unsigned, string_view>(42, ""));
        Assert(read_opt<unsigned long>("42").value() == std::make_pair<unsigned long, string_view>(42, ""));
        Assert(read_opt<unsigned long long>("42").value() == std::make_pair<unsigned long long, string_view>(42, ""));

        Assert(read_opt<float>("1.0").value() == std::make_pair<float, string_view>(1.0, ""));
        Assert(read_opt<double>("1.0").value() == std::make_pair<double, string_view>(1.0, ""));
        Assert(read_opt<long double>("1.0").value() == std::make_pair<long double, string_view>(1.0, ""));

        Assert(read_opt<char>("abc").value() == std::make_pair<char, string_view>('a', "bc"));
        Assert(read_opt<char>("   a").value() == std::make_pair<char, string_view>('a', ""));
        Assert(read_opt<char>("   ") == std::experimental::nullopt);

        Assert(read_opt<bool>("true.").value() == std::make_pair<bool, string_view>(true, "."));
        Assert(read_opt<bool>("false.").value() == std::make_pair<bool, string_view>(false, "."));
        Assert(read_opt<bool>("  true").value() == std::make_pair<bool, string_view>(true, ""));
    }
}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

