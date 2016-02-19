#include <cat/pretty.hpp>
#include <cat/functional.hpp>
#include <cat/bits/type.hpp>

#include "yats.hpp"

using namespace cat;

// Tests:
//

auto g = yats::Group("test_pretty")

    .Single("simple", []
    {
        std::cout << pretty(static_cast<int16_t>(42)) << std::endl;
        std::cout << pretty(static_cast<uint16_t>(42)) << std::endl;

        std::cout << pretty(42) << std::endl;
        std::cout << pretty(42l) << std::endl;
        std::cout << pretty(42ll) << std::endl;
        std::cout << pretty(42u) << std::endl;
        std::cout << pretty(42ul) << std::endl;
        std::cout << pretty(42ull) << std::endl;

        std::cout << pretty(42.0f) << std::endl;
        std::cout << pretty(42.0) << std::endl;
        std::cout << pretty(42.0l) << std::endl;

        std::cout << pretty("Hello World") << std::endl;
        std::cout << pretty(std::string("Hello World")) << std::endl;
        std::cout << pretty(std::basic_string<int>{1,2,3}) << std::endl;
        std::cout << pretty(string_view("Hello World")) << std::endl;

        std::cout << pretty('f') << std::endl;
        std::cout << pretty(static_cast<unsigned char>('x')) << std::endl;

        std::cout << pretty(true) << std::endl;
        std::cout << pretty((const char *)"Hello world") << std::endl;
        std::cout << pretty((const int *)0xbadbee) << std::endl;
        std::cout << pretty(std::make_unique<int>(42)) << std::endl;
        std::cout << pretty(std::make_shared<int>(42)) << std::endl;

        std::cout << pretty(std::make_pair('c', "is a char")) << std::endl;
        std::cout << pretty(std::make_tuple(1, "is", true)) << std::endl;

        std::cout << pretty(std::chrono::seconds(1)) << std::endl;
        std::cout << pretty(std::chrono::system_clock::now()) << std::endl;

        std::cout << pretty(std::initializer_list<int>{1,2,3}) << std::endl;
        std::cout << pretty(std::array<int, 3>{{1,2,3}}) << std::endl;
        std::cout << pretty(std::vector<int>{1,2,3}) << std::endl;
        std::cout << pretty(std::list<int>{1,2,3}) << std::endl;
        std::cout << pretty(std::forward_list<int>{1,2,3}) << std::endl;
        std::cout << pretty(std::deque<int>{1,2,3}) << std::endl;

        std::cout << pretty(std::set<int>{1,2,3}) << std::endl;
        std::cout << pretty(std::unordered_set<int>{1,2,3}) << std::endl;
        std::cout << pretty(std::multiset<int>{1,1,2,2,3,3}) << std::endl;
        std::cout << pretty(std::unordered_multiset<int>{1,1,2,2,3,3}) << std::endl;

        std::cout << pretty(std::map<int, std::string>{{1, "hello"},{2,"world"}}) << std::endl;
        std::cout << pretty(std::unordered_map<int, std::string>{{1, "hello"},{2,"world"}}) << std::endl;
        std::cout << pretty(std::multimap<int, std::string>{{42, "hello"},{42,"world"}}) << std::endl;
        std::cout << pretty(std::unordered_multimap<int, std::string>{{42, "hello"},{42,"world"}}) << std::endl;

        std::cout << pretty(nullptr) << std::endl;

        std::cout << pretty(std::integral_constant<int, 42>()) << std::endl;
        std::cout << pretty(std::integral_constant<bool, true>()) << std::endl;
        std::cout << pretty(std::make_integer_sequence<int, 10>{}) << std::endl;

        std::cout << pretty(make_optional(42)) << std::endl;
        std::cout << pretty(optional<int>()) << std::endl;
        std::cout << pretty(nullopt) << std::endl;

    });

int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

