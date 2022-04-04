#include <cat/show.hpp>
#include <cat/functional.hpp>
#include <cat/bits/type.hpp>

#include "yats.hpp"

using namespace cat;

// Tests:
//

auto g = yats::Group("test_show")

    .Single("simple", []
    {
        print(static_cast<int16_t>(42));
        print(static_cast<uint16_t>(42));

        print(42);
        print(42l);
        print(42ll);
        print(42u);
        print(42ul);
        print(42ull);

        print(42.0f);
        print(42.0);
        print(42.0l);

        print("Hello World");
        print(std::string("Hello World"));
        print(std::basic_string<int>{1,2,3});
        print(std::string_view("Hello World"));

        print('f');
        print(static_cast<unsigned char>('x'));

        print(true);
        print((const char *)"Hello world");
        print((const int *)0xbadbee);
        print(std::make_unique<int>(42));
        print(std::make_shared<int>(42));

        print(std::make_pair('c', "is a char"));
        print(std::make_tuple(1, "is", true));

        print(std::chrono::seconds(1));
        print(std::chrono::system_clock::now());

        print(std::initializer_list<int>{1,2,3});
        print(std::array<int, 3>{{1,2,3}});
        print(std::vector<int>{1,2,3});
        print(std::list<int>{1,2,3});
        print(std::forward_list<int>{1,2,3});
        print(std::deque<int>{1,2,3});

        print(std::set<int>{1,2,3});
        print(std::unordered_set<int>{1,2,3});
        print(std::multiset<int>{1,1,2,2,3,3});
        print(std::unordered_multiset<int>{1,1,2,2,3,3});

        print(std::map<int, std::string>{{1, "hello"},{2,"world"}});
        print(std::unordered_map<int, std::string>{{1, "hello"},{2,"world"}});
        print(std::multimap<int, std::string>{{42, "hello"},{42,"world"}});
        print(std::unordered_multimap<int, std::string>{{42, "hello"},{42,"world"}});

        print(nullptr);

        print(std::integral_constant<int, 42>());
        print(std::integral_constant<bool, true>());
        print(std::make_integer_sequence<int, 10>{});

        print(std::make_optional(42));
        print(std::optional<int>());
        print(std::nullopt);

        std::cout << "curry: ";
        curry(cat::print)(std::make_optional(42));
    });

int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}
