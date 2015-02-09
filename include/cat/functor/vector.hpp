#pragma once

#include <vector>

#include <cat/functor/functor.hpp>

namespace cat
{
    // vector is a functor:

    template <> struct is_functor<std::vector> : std::true_type { };

    // vector instance:

    template <typename Fun, typename A, typename Alloc>
    struct FunctorInstance<std::vector, Fun, A, Alloc> : Functor<std::vector>::Class<Fun, A, Alloc>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        auto fmap(Fun f, std::vector<A, Alloc> const &xs) -> std::vector<B, rebind_t<Alloc, B> > final
        {
            std::vector<B, rebind_t<Alloc, B> > out;
            out.reserve(xs.size());

            for(auto & x : xs)
                out.push_back(f(x));

            return out;
        }
    };



} // namespace cat

