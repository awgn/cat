#include <cat/show.hpp>
#include <cat/pretty.hpp>

#include "yats.hpp"

using namespace cat;

// Tests:
//

auto g = yats::Group("test_existential")

    .Single("forall_single", []
    {
        std::vector<forall<Show>> v;

        v.emplace_back(1);
        v.emplace_back(std::string{"hello"});
        v.emplace_back(std::vector<int>{1,2,3});

        for (auto & e : v)
            std::cout << show(e) << std::endl;

    })

    .Single("forall_multiple", []
    {
        std::vector<forall<Show, Pretty>> v;

        v.emplace_back(1);
        v.emplace_back(std::string{"hello"});
        v.emplace_back(std::vector<int>{1,2,3});

        for (auto & e : v)
            std::cout << show(e) << " -> " << pretty(e) << std::endl;

    })

    ;

int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

