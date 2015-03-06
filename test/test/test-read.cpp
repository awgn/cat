#include <cat/read.hpp>
#include <cat/show.hpp>
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
        Assert(reads<short>("...") == nullopt);

        Assert(reads<unsigned char>(" test").value() == std::make_pair<unsigned char, string_view>('t', "est"));

        Assert(reads<short>("123").value() == std::make_pair<short, string_view>(123, ""));
        Assert(reads<int>("123").value() == std::make_pair<int, string_view>(123, ""));

        Assert(reads<long>("-123").value() == std::make_pair<long, string_view>(-123, ""));
        Assert(reads<long long>("-123").value() == std::make_pair<long long, string_view>(-123, ""));

        Assert(reads<unsigned short>("42").value() == std::make_pair<unsigned short, string_view>(42, ""));
        Assert(reads<unsigned>("42").value() == std::make_pair<unsigned, string_view>(42, ""));
        Assert(reads<unsigned long>("42").value() == std::make_pair<unsigned long, string_view>(42, ""));
        Assert(reads<unsigned long long>("42").value() == std::make_pair<unsigned long long, string_view>(42, ""));

        Assert(reads<float>("1.0").value() == std::make_pair<float, string_view>(1.0, ""));
        Assert(reads<double>("1.0").value() == std::make_pair<double, string_view>(1.0, ""));
        Assert(reads<long double>("1.0").value() == std::make_pair<long double, string_view>(1.0, ""));

        Assert(reads<char>("abc").value() == std::make_pair<char, string_view>('a', "bc"));
        Assert(reads<char>("   a").value() == std::make_pair<char, string_view>('a', ""));
        Assert(reads<char>("   ") == nullopt);

        Assert(reads<bool>("true.").value() == std::make_pair<bool, string_view>(true, "."));
        Assert(reads<bool>("false.").value() == std::make_pair<bool, string_view>(false, "."));
        Assert(reads<bool>("  true").value() == std::make_pair<bool, string_view>(true, ""));
    }


    Test(pair)
    {
        Assert(reads<std::pair<char, int>>("x") == nullopt);
        Assert(reads<std::pair<char, int>>(" (  x )") == nullopt);
        Assert(reads<std::pair<char, int>>(" (  x )") == nullopt);
        Assert(reads<std::pair<char, int>>("(x 42)").value() == std::make_pair< std::pair<char,int>, string_view>( {'x', 42 }, "" ));
        Assert(reads<std::pair<char, int>>(" (  x 42)!").value() == std::make_pair< std::pair<char,int>, string_view>( {'x', 42 }, "!" ));
    }


    Test(tuple)
    {
        Assert(reads<std::tuple<int>>("x") == nullopt);
        Assert(reads<std::tuple<int>>(" ( 42)") == std::make_pair(std::make_tuple(42), string_view{""}));
        Assert(reads<std::tuple<char, int, double>>("x") == nullopt);
        Assert(reads<std::tuple<char, int, double>>(" (  x )") == nullopt);
        Assert(reads<std::tuple<char, int, double>>(" (  x )") == nullopt);
        Assert(reads<std::tuple<char, int, double>>("(x 42 1.0)").value() == std::make_pair( std::make_tuple<char, int, double>('x', 42, 1.0 ), string_view{""} ));
        Assert(reads<std::tuple<char, int, double>>(" (  x 42 1.0)!").value() == std::make_pair( std::make_tuple<char, int, double>('x', 42, 1.0 ), string_view{"!"} ));
    }

    Test(optional)
    {
        Assert(reads<optional<int>>("x") == nullopt);
        Assert(reads<optional<int>>(" (  x )") == nullopt);
        Assert(reads<optional<int>>(" (  )").value() == std::pair<optional<int>, string_view>());
        Assert(reads<optional<int>>(" (  11 )") == std::make_pair(make_optional(11), string_view{""}));


        Assert(reads<std::tuple<optional<int>>>("x") == nullopt);
        Assert(reads<std::tuple<optional<int>>>("()") == nullopt);
        Assert(reads<std::tuple<optional<int>>>("(())").value() == std::make_pair(std::tuple<optional<int>>{}, string_view{""}));
        Assert(reads<std::tuple<optional<int>>>("((42) )!").value() == std::make_pair(std::tuple<optional<int>>{42}, string_view{"!"}));
    }

    Test(string)
    {
        Assert(reads<std::string>("") == nullopt);
        Assert(reads<std::string>("   ") == nullopt);
        Assert(reads<std::string>(" abc ").value() == std::make_pair(std::string("abc"), string_view{" "}));
        Assert(reads<std::string>(R"(""!)") == std::make_pair(std::string(""), string_view{"!"}));
        Assert(reads<std::string>(R"("x")") == std::make_pair(std::string("x"), string_view{""}));
        Assert(reads<std::string>(R"(   "abc" )") == std::make_pair(std::string("abc"), string_view{" "}));
        Assert(reads<std::string>(R"(   "abc\"123" )") == std::make_pair(std::string(R"(abc"123)"), string_view{" "}));
        Assert(reads<std::string>(R"(   "\\")") == std::make_pair(std::string(R"(\)"), string_view{""}));

        Assert(reads<std::string>(R"(test")") == std::make_pair(std::string(R"(test")"), string_view{""}));
        Assert(reads<std::string>(R"("test)") == nullopt);
    }
    }
}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

