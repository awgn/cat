#include <cat/show.hpp>
#include <cat/pretty.hpp>

#include "yats.hpp"

using namespace cat;

// Tests:
//

auto g = yats::Group("test_existential")

    .Single("forall_single", []
    {
#if (__GNUC__ >= 5)
        std::vector<forall<Show>> v;

        v.emplace_back(1);
        v.emplace_back(std::string{"hello"});
        v.emplace_back(std::vector<int>{1,2,3});

        for (auto & e : v)
            std::cout << show(e) << std::endl;
#else
#warning "existential quantification enabled for gcc 5 or higher"
#endif

    })

    .Single("forall_multiple", []
    {
#if (__GNUC__ >= 5)
        std::vector<forall<Show, Pretty>> v;

        v.emplace_back(1);
        v.emplace_back(std::string{"hello"});
        v.emplace_back(std::vector<int>{1,2,3});

        for (auto & e : v)
            std::cout << show(e) << " -> " << pretty(e) << std::endl;
#endif
    })

    ;

int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

