#include <cat/optional.hpp>

#include <yats.hpp>

using namespace yats;
using namespace cat;


// Tests:
//

auto g = Group("iterator")

    .Single("test_optional", []
    {
        Assert(maybe(42, [](int n) { return n+1; }, std::make_optional(1)), is_equal_to(2));

    })

    .Single("map_optionals", []
    {
        std::vector<int> v {1,2,3};

        Assert(map_optional([](int n) { return n > 0 ? std::make_optional(n) : std::optional<int>{}; }, v) == std::vector<int>{1,2,3});
        Assert(map_optional([](int n) { return n > 0 ? std::make_optional(n) : std::optional<int>{}; }, std::move(v)) == std::vector<int>{1,2,3});
    })

    .Single("cat_optionals", []
    {
        std::vector<std::optional<int>> v {std::make_optional(1),
                                      std::make_optional(2),
                                      std::make_optional(3),
                                      std::optional<int>{} };

        Assert(cat_optionals(v) == std::vector<int>{1,2,3});
        Assert(cat_optionals(std::move(v)) == std::vector<int>{1,2,3});
    });


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}
