#include <cat/bifunctor.hpp>

#include <yats.hpp>

using namespace yats;
using namespace cat;


// Tests:
//

Context(functor)
{
    template <template <typename...> class F, typename ...Ts>
    void functor_constraint(F<Ts...> const &)
    {
        static_assert(is_bifunctor<F>::value, "F: not a bifunctor!");
    }

    Test(bifunctor_pair)
    {
        auto p = std::make_pair(std::string("hello"), std::string("world!"));

        auto b = bimap([](const std::string &s) -> size_t { return s.size(); },
                       [](const std::string &s) -> size_t { return s.size(); },
                       p);

        auto f = bifirst ([](const std::string &s) -> size_t { return s.size(); }, p);
        auto s = bisecond([](const std::string &s) -> size_t { return s.size(); }, p);

        Assert(b, is_equal_to(std::pair<size_t, size_t>{5, 6}));
        Assert(f, is_equal_to(std::pair<size_t, std::string>{5, "world!"}));
        Assert(s, is_equal_to(std::pair<std::string, size_t>{"hello", 6}));
    }


    Test(bifunctor_pair2)
    {
        auto p = std::make_pair(std::string("hello"), std::string("world!"));

        auto b = currying(bimap)([](const std::string &s) -> size_t { return s.size();})
                                ([](const std::string &s) -> size_t { return s.size(); })
                                (p);

        auto f = currying(bifirst)([](const std::string &s) -> size_t { return s.size(); }, p);
        auto s = currying(bisecond)([](const std::string &s) -> size_t { return s.size(); }, p);

        Assert(b, is_equal_to(std::pair<size_t, size_t>{5, 6}));
        Assert(f, is_equal_to(std::pair<size_t, std::string>{5, "world!"}));
        Assert(s, is_equal_to(std::pair<std::string, size_t>{"hello", 6}));
    }


    Test(functor_constraint)
    {
        functor_constraint( std::pair<int, std::string>{} );
    }

}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

