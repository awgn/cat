#include <cat/read.hpp>
#include <cat/show.hpp>
#include <cat/bits/type.hpp>

#include <utility>
#include <string_view>

#include "yats.hpp"

using namespace yats;
using namespace cat;
using namespace std::literals::chrono_literals;


// Tests:
//

auto g = Group("test_read")

    .Single("simple", []
    {
        Assert(reads<short>("...") == std::nullopt);

        Assert(reads<unsigned char>(" test").value() == std::make_pair<unsigned char, std::string_view>('t', "est"));

        Assert(reads<short>("123").value() == std::make_pair<short, std::string_view>(123, ""));
        Assert(reads<int>("123").value() == std::make_pair<int, std::string_view>(123, ""));

        Assert(reads<long>("-123").value() == std::make_pair<long, std::string_view>(-123, ""));
        Assert(reads<long long>("-123").value() == std::make_pair<long long, std::string_view>(-123, ""));

        Assert(reads<unsigned short>("42").value() == std::make_pair<unsigned short, std::string_view>(42, ""));
        Assert(reads<unsigned>("42").value() == std::make_pair<unsigned, std::string_view>(42, ""));
        Assert(reads<unsigned long>("42").value() == std::make_pair<unsigned long, std::string_view>(42, ""));
        Assert(reads<unsigned long long>("42").value() == std::make_pair<unsigned long long, std::string_view>(42, ""));

        Assert(reads<float>("1.0").value() == std::make_pair<float, std::string_view>(1.0, ""));
        Assert(reads<double>("1.0").value() == std::make_pair<double, std::string_view>(1.0, ""));
        Assert(reads<long double>("1.0").value() == std::make_pair<long double, std::string_view>(1.0, ""));

        Assert(reads<char>("abc").value() == std::make_pair<char, std::string_view>('a', "bc"));
        Assert(reads<char>("   a").value() == std::make_pair<char, std::string_view>('a', ""));
        Assert(reads<char>("   ") == std::nullopt);

        Assert(reads<bool>("true.").value() == std::make_pair<bool, std::string_view>(true, "."));
        Assert(reads<bool>("false.").value() == std::make_pair<bool, std::string_view>(false, "."));
        Assert(reads<bool>("  true").value() == std::make_pair<bool, std::string_view>(true, ""));
    })


    .Single("pair", []
    {
        Assert(reads<std::pair<char, int>>("x") == std::nullopt);
        Assert(reads<std::pair<char, int>>(" (  x )") == std::nullopt);
        Assert(reads<std::pair<char, int>>(" (  x )") == std::nullopt);
        Assert(reads<std::pair<char, int>>("(x 42)").value() == std::make_pair< std::pair<char,int>, std::string_view>( {'x', 42 }, "" ));
        Assert(reads<std::pair<char, int>>(" (  x 42)!").value() == std::make_pair< std::pair<char,int>, std::string_view>( {'x', 42 }, "!" ));
    })


    .Single("tuple", []
    {
        Assert(reads<std::tuple<int>>("x") == std::nullopt);
        Assert(reads<std::tuple<int>>(" ( 42)") == std::make_pair(std::make_tuple(42), std::string_view{""}));
        Assert(reads<std::tuple<char, int, double>>("x") == std::nullopt);
        Assert(reads<std::tuple<char, int, double>>(" (  x )") == std::nullopt);
        Assert(reads<std::tuple<char, int, double>>(" (  x )") == std::nullopt);
        Assert(reads<std::tuple<char, int, double>>("(x 42 1.0)").value() == std::make_pair( std::make_tuple<char, int, double>('x', 42, 1.0 ), std::string_view{""} ));
        Assert(reads<std::tuple<char, int, double>>(" (  x 42 1.0)!").value() == std::make_pair( std::make_tuple<char, int, double>('x', 42, 1.0 ), std::string_view{"!"} ));
    })

    .Single("std::optional", []
    {
        Assert(reads<std::optional<int>>("x") == std::nullopt);
        Assert(reads<std::optional<int>>(" (  x )") == std::nullopt);
        Assert(reads<std::optional<int>>(" (  )").value() == std::pair<std::optional<int>, std::string_view>());
        Assert(reads<std::optional<int>>(" (  11 )") == std::make_pair(std::make_optional(11), std::string_view{""}));


        Assert(reads<std::tuple<std::optional<int>>>("x") == std::nullopt);
        Assert(reads<std::tuple<std::optional<int>>>("()") == std::nullopt);
        Assert(reads<std::tuple<std::optional<int>>>("(())").value() == std::make_pair(std::tuple<std::optional<int>>{}, std::string_view{""}));
        Assert(reads<std::tuple<std::optional<int>>>("((42) )!").value() == std::make_pair(std::tuple<std::optional<int>>{42}, std::string_view{"!"}));
    })

    .Single("string", []
    {
        Assert(reads<std::string>("") == std::nullopt);
        Assert(reads<std::string>("   ") == std::nullopt);
        Assert(reads<std::string>("abc").value() == std::make_pair(std::string("abc"), std::string_view{""}));
        Assert(reads<std::string>(" abc").value() == std::make_pair(std::string("abc"), std::string_view{""}));
        Assert(reads<std::string>(" abc  ").value() == std::make_pair(std::string("abc"), std::string_view{"  "}));
        Assert(reads<std::string>("abc  ").value() == std::make_pair(std::string("abc"), std::string_view{"  "}));
        Assert(reads<std::string>(R"(""!)") == std::make_pair(std::string(""), std::string_view{"!"}));
        Assert(reads<std::string>(R"(   ""!)") == std::make_pair(std::string(""), std::string_view{"!"}));
        Assert(reads<std::string>(R"(   ""   !)") == std::make_pair(std::string(""), std::string_view{"   !"}));
        Assert(reads<std::string>(R"("x")") == std::make_pair(std::string("x"), std::string_view{""}));

        Assert(reads<std::string>(R"("abc")") == std::make_pair(std::string("abc"), std::string_view{""}));
        Assert(reads<std::string>(R"(   "abc")") == std::make_pair(std::string("abc"), std::string_view{""}));
        Assert(reads<std::string>(R"("abc" )") == std::make_pair(std::string("abc"), std::string_view{" "}));
        Assert(reads<std::string>(R"(   "abc" )") == std::make_pair(std::string("abc"), std::string_view{" "}));

        Assert(reads<std::string>(R"(   "abc\"123" )") == std::make_pair(std::string(R"(abc"123)"), std::string_view{" "}));
        Assert(reads<std::string>(R"(   "\\")") == std::make_pair(std::string(R"(\)"), std::string_view{""}));

        Assert(reads<std::string>(R"(test")") == std::make_pair(std::string(R"(test")"), std::string_view{""}));
        Assert(reads<std::string>(R"("test)") == std::nullopt);
    })

    .Single("duration", []
    {
        Assert(reads<std::chrono::nanoseconds>("x") == std::nullopt);
        Assert(reads<std::chrono::nanoseconds>("42") == std::nullopt);
        Assert(reads<std::chrono::nanoseconds>("42_") == std::nullopt);
        Assert(reads<std::chrono::nanoseconds>("42_ns") == std::make_pair(42ns, std::string_view{""}));
        Assert(reads<std::chrono::microseconds>("42_us") == std::make_pair(42us, std::string_view{""}));
        Assert(reads<std::chrono::milliseconds>("42_ms") == std::make_pair(42ms, std::string_view{""}));
        Assert(reads<std::chrono::seconds>("42_s") == std::make_pair(42s, std::string_view{""}));
        Assert(reads<std::chrono::minutes>("42_min") == std::make_pair(42min, std::string_view{""}));
        Assert(reads<std::chrono::hours>("42_h") == std::make_pair(42h, std::string_view{""}));

        Assert(reads<std::chrono::system_clock::time_point>("x") == std::nullopt);
        Assert(reads<std::chrono::system_clock::time_point>("42") == std::nullopt);
        Assert(reads<std::chrono::system_clock::time_point>("42_") == std::nullopt);
        Assert(reads<std::chrono::system_clock::time_point>("42_us") == std::make_pair(std::chrono::system_clock::time_point(
                    std::chrono::duration_cast<std::chrono::system_clock::duration>(42us)), std::string_view{""}));
    })

    .Single("container", []
    {
        Assert(reads<std::vector<int>>("  ") == std::nullopt);
        Assert(reads<std::vector<int>>("[") == std::nullopt);
        Assert(reads<std::vector<int>>("]") == std::nullopt);
        Assert(reads<std::vector<int>>("[ ]") == std::make_pair(std::vector<int>{}, std::string_view{""}));
        Assert(reads<std::vector<int>>("[ 1 ]") == std::make_pair(std::vector<int>{1}, std::string_view{""}));
        Assert(reads<std::vector<int>>("[ 1 2 3 ]") == std::make_pair(std::vector<int>{1, 2, 3}, std::string_view{""}));

        Assert(reads<std::array<int,1>>("[ 1 2 3 ]") == std::nullopt);
        Assert(reads<std::array<int,2>>("[ 1 2 3 ]") == std::nullopt);
        Assert(reads<std::array<int,3>>("[ 1 2 3 ]") == std::make_pair(std::array<int, 3>{{1, 2, 3}}, std::string_view{""}));

        Assert(reads<std::list<int>>("[ 1 2 3 ]") == std::make_pair(std::list<int>{1, 2, 3}, std::string_view{""}));
        Assert(reads<std::forward_list<int>>("[ 1 2 3 ]") == std::make_pair(std::forward_list<int>{1, 2, 3}, std::string_view{""}));
        Assert(reads<std::deque<int>>("[ 1 2 3 ]") == std::make_pair(std::deque<int>{1, 2, 3}, std::string_view{""}));

        Assert(reads<std::set<int>>("[ 1 2 3 ]") == std::make_pair(std::set<int>{1, 2, 3}, std::string_view{""}));
        Assert(reads<std::unordered_set<int>>("[ 1 2 3 ]") == std::make_pair(std::unordered_set<int>{1, 2, 3}, std::string_view{""}));
        Assert(reads<std::multiset<int>>("[ 1 1 2 2 3 3 ]") == std::make_pair(std::multiset<int>{1, 1, 2, 2, 3, 3}, std::string_view{""}));
        Assert(reads<std::unordered_multiset<int>>("[ 1 1 2 2 3 3 ]") == std::make_pair(std::unordered_multiset<int>{1, 1, 2, 2, 3, 3}, std::string_view{""}));

        Assert(reads<std::map<int, std::string>>("[ (1 \"hello\") (2 \"world\") ]") == std::make_pair(std::map<int, std::string>{{1, "hello"}, {2,"world"}}, std::string_view{""}));
        Assert(reads<std::unordered_map<int, std::string>>("[ (1 \"hello\") (2 \"world\") ]") == std::make_pair(std::unordered_map<int, std::string>{{1, "hello"}, {2,"world"}}, std::string_view{""}));
        Assert(reads<std::multimap<int, std::string>>("[ (1 \"hello\") (2 \"world\") ]") == std::make_pair(std::multimap<int, std::string>{{1, "hello"}, {2,"world"}}, std::string_view{""}));
        Assert(reads<std::unordered_multimap<int, std::string>>("[ (1 \"hello\") (2 \"world\") ]") == std::make_pair(std::unordered_multimap<int, std::string>{{1, "hello"}, {2,"world"}}, std::string_view{""}));
    });

int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}
