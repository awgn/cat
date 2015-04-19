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
        Assert(reads<std::string>("abc").value() == std::make_pair(std::string("abc"), string_view{""}));
        Assert(reads<std::string>(" abc").value() == std::make_pair(std::string("abc"), string_view{""}));
        Assert(reads<std::string>(" abc  ").value() == std::make_pair(std::string("abc"), string_view{"  "}));
        Assert(reads<std::string>("abc  ").value() == std::make_pair(std::string("abc"), string_view{"  "}));
        Assert(reads<std::string>(R"(""!)") == std::make_pair(std::string(""), string_view{"!"}));
        Assert(reads<std::string>(R"(   ""!)") == std::make_pair(std::string(""), string_view{"!"}));
        Assert(reads<std::string>(R"(   ""   !)") == std::make_pair(std::string(""), string_view{"   !"}));
        Assert(reads<std::string>(R"("x")") == std::make_pair(std::string("x"), string_view{""}));

        Assert(reads<std::string>(R"("abc")") == std::make_pair(std::string("abc"), string_view{""}));
        Assert(reads<std::string>(R"(   "abc")") == std::make_pair(std::string("abc"), string_view{""}));
        Assert(reads<std::string>(R"("abc" )") == std::make_pair(std::string("abc"), string_view{" "}));
        Assert(reads<std::string>(R"(   "abc" )") == std::make_pair(std::string("abc"), string_view{" "}));

        Assert(reads<std::string>(R"(   "abc\"123" )") == std::make_pair(std::string(R"(abc"123)"), string_view{" "}));
        Assert(reads<std::string>(R"(   "\\")") == std::make_pair(std::string(R"(\)"), string_view{""}));

        Assert(reads<std::string>(R"(test")") == std::make_pair(std::string(R"(test")"), string_view{""}));
        Assert(reads<std::string>(R"("test)") == nullopt);
    }

    using namespace std::literals::chrono_literals;

    Test(duration)
    {
        Assert(reads<std::chrono::nanoseconds>("x") == nullopt);
        Assert(reads<std::chrono::nanoseconds>("42") == nullopt);
        Assert(reads<std::chrono::nanoseconds>("42_") == nullopt);
        Assert(reads<std::chrono::nanoseconds>("42_ns") == std::make_pair(42ns, string_view{""}));
        Assert(reads<std::chrono::microseconds>("42_us") == std::make_pair(42us, string_view{""}));
        Assert(reads<std::chrono::milliseconds>("42_ms") == std::make_pair(42ms, string_view{""}));
        Assert(reads<std::chrono::seconds>("42_s") == std::make_pair(42s, string_view{""}));
        Assert(reads<std::chrono::minutes>("42_min") == std::make_pair(42min, string_view{""}));
        Assert(reads<std::chrono::hours>("42_h") == std::make_pair(42h, string_view{""}));

        Assert(reads<std::chrono::system_clock::time_point>("x") == nullopt);
        Assert(reads<std::chrono::system_clock::time_point>("42") == nullopt);
        Assert(reads<std::chrono::system_clock::time_point>("42_") == nullopt);
        Assert(reads<std::chrono::system_clock::time_point>("42_us") == std::make_pair(std::chrono::system_clock::time_point(
                    std::chrono::duration_cast<std::chrono::system_clock::duration>(42us)), string_view{""}));
    }

    Test(container)
    {
        Assert(reads<std::vector<int>>("  ") == nullopt);
        Assert(reads<std::vector<int>>("[") == nullopt);
        Assert(reads<std::vector<int>>("]") == nullopt);
        Assert(reads<std::vector<int>>("[ ]") == std::make_pair(std::vector<int>{}, string_view{""}));
        Assert(reads<std::vector<int>>("[ 1 ]") == std::make_pair(std::vector<int>{1}, string_view{""}));
        Assert(reads<std::vector<int>>("[ 1 2 3 ]") == std::make_pair(std::vector<int>{1, 2, 3}, string_view{""}));

        Assert(reads<std::array<int,1>>("[ 1 2 3 ]") == nullopt);
        Assert(reads<std::array<int,2>>("[ 1 2 3 ]") == nullopt);
        Assert(reads<std::array<int,3>>("[ 1 2 3 ]") == std::make_pair(std::array<int, 3>{{1, 2, 3}}, string_view{""}));

        Assert(reads<std::list<int>>("[ 1 2 3 ]") == std::make_pair(std::list<int>{1, 2, 3}, string_view{""}));
        Assert(reads<std::forward_list<int>>("[ 1 2 3 ]") == std::make_pair(std::forward_list<int>{1, 2, 3}, string_view{""}));
        Assert(reads<std::deque<int>>("[ 1 2 3 ]") == std::make_pair(std::deque<int>{1, 2, 3}, string_view{""}));

        Assert(reads<std::set<int>>("[ 1 2 3 ]") == std::make_pair(std::set<int>{1, 2, 3}, string_view{""}));
        Assert(reads<std::unordered_set<int>>("[ 1 2 3 ]") == std::make_pair(std::unordered_set<int>{1, 2, 3}, string_view{""}));
        Assert(reads<std::multiset<int>>("[ 1 1 2 2 3 3 ]") == std::make_pair(std::multiset<int>{1, 1, 2, 2, 3, 3}, string_view{""}));
        Assert(reads<std::unordered_multiset<int>>("[ 1 1 2 2 3 3 ]") == std::make_pair(std::unordered_multiset<int>{1, 1, 2, 2, 3, 3}, string_view{""}));

        Assert(reads<std::map<int, std::string>>("[ (1 \"hello\") (2 \"world\") ]") == std::make_pair(std::map<int, std::string>{{1, "hello"}, {2,"world"}}, string_view{""}));
        Assert(reads<std::unordered_map<int, std::string>>("[ (1 \"hello\") (2 \"world\") ]") == std::make_pair(std::unordered_map<int, std::string>{{1, "hello"}, {2,"world"}}, string_view{""}));
        Assert(reads<std::multimap<int, std::string>>("[ (1 \"hello\") (2 \"world\") ]") == std::make_pair(std::multimap<int, std::string>{{1, "hello"}, {2,"world"}}, string_view{""}));
        Assert(reads<std::unordered_multimap<int, std::string>>("[ (1 \"hello\") (2 \"world\") ]") == std::make_pair(std::unordered_multimap<int, std::string>{{1, "hello"}, {2,"world"}}, string_view{""}));
    }
}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

