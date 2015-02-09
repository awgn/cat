#include <utility>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>

/* Functor Typeclass */

template <template <typename ...> class F>
struct Functor
{
    template <typename Fun, typename A>
    struct Class
    {
        virtual auto fmap(Fun f, F<A> const & f_a) -> F<decltype(f(std::declval<A>()))> = 0;
    };
};


template <template <typename ...> class F, typename ...> struct FunctorInstance;


template <template <typename ...> class F, typename Fun, typename A>
auto fmap(Fun f, F<A> const &xs)
{
    return FunctorInstance<F, Fun, A>{}.fmap(f, xs);
}


/* Vector instance */

template <typename Fun, typename A>
struct FunctorInstance<std::vector, Fun, A> : Functor<std::vector>::Class<Fun, A>
{
    using B = decltype(std::declval<Fun>()(std::declval<A>()));

    auto fmap(Fun f, std::vector<A> const &xs) -> std::vector<B> final
    {
        std::vector<B> out;
        out.reserve(xs.size());

        for(auto & x : xs)
            out.push_back(f(x));

        return out;
    }
};


/* ---------------------------------------------------- */


struct id
{
    template <typename T>
    auto operator()(T && arg)
    {
        return std::forward<T>(arg);
    }
};


int
main(int argc, char *argv[])
{
    std::vector<std::string> a{"a", "ab", "abc"};
    auto b = fmap([](std::string const & s) { return s.size(); }, a);
    return 0;
}

