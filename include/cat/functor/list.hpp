#pragma once

#include <list>

#include <cat/functor/functor.hpp>

namespace cat
{
    // list is a functor:
    //

    template <> struct is_functor<std::list> : std::true_type { };

    // list instance:
    //

    template <typename Fun, typename A, typename Alloc>
    struct FunctorInstance<std::list, Fun, A, Alloc> : Functor<std::list>::Class<Fun, A, Alloc>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        std::list<B, rebind_t<Alloc, B>>
        fmap(Fun f, std::list<A, Alloc> const &xs) final
        {
            std::list<B, rebind_t<Alloc, B> > out;

            for(auto & x : xs)
                out.push_back(f(x));

            return out;
        }
    };

} // namespace cat

