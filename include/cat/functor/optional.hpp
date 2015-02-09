#pragma once

#include <experimental/optional>

#include <cat/functor/functor.hpp>

namespace cat
{
    // experimental::optional instance:

    template <typename Fun, typename A>
    struct FunctorInstance<std::experimental::optional, Fun, A> : Functor<std::experimental::optional>::Class<Fun, A>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        auto fmap(Fun f, std::experimental::optional<A> const &xs) -> std::experimental::optional<B> final
        {
            using type = decltype(f(*xs));

            if (xs)
                return std::experimental::make_optional<type>(f(*xs));

            return std::experimental::optional<type>();
        }
    };

} // namespace cat

