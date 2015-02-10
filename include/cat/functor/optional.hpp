#pragma once

#include <experimental/optional>

#include <cat/functor/functor.hpp>

namespace cat
{
    // optional is a functor:
    //

    template <> struct is_functor<std::experimental::optional> : std::true_type { };

    // experimental::optional instance:
    //

    template <typename Fun, typename A>
    struct FunctorInstance<std::experimental::optional, Fun, A> : Functor<std::experimental::optional>::Class<Fun, A>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        std::experimental::optional<B>
        fmap(Fun f, std::experimental::optional<A> const &xs) final
        {
            using type = decltype(f(*xs));

            if (xs)
                return std::experimental::make_optional(f(*xs));

            return std::experimental::optional<type>();
        }
    };

} // namespace cat

