#pragma once

#include <tuple>
#include <utility>

#include <cat/functor/functor.hpp>

namespace cat
{
    // std::pair is a functor:
    //

    template <> struct is_functor<std::pair> : std::true_type { };

    // std::pair instance:
    //

    template <typename Fun, typename P, typename A>
    struct FunctorInstance<std::pair, Fun, P, A> : Functor<std::pair>::Class1<Fun, P, A>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        std::pair<P, B>
        fmap(Fun f, std::pair<P, A> const &xs) final
        {
            return std::make_pair(xs.first, f(xs.second));
        }
    };

} // namespace cat



