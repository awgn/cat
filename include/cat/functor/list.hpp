#pragma once

#include <list>

#include <cat/functor/functor.hpp>

namespace cat
{
    // list instance:

    template <typename Fun, typename A, typename Alloc>
    struct FunctorInstance<std::list, Fun, A, Alloc> : Functor<std::list>::Class<Fun, A, Alloc>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        auto fmap(Fun f, std::list<A, Alloc> const &xs) -> std::list<B, rebind_t<Alloc, B> > final
        {
            std::list<B, rebind_t<Alloc, B> > out;

            for(auto & x : xs)
                out.push_back(f(x));

            return out;
        }
    };

} // namespace cat

