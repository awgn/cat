#pragma once

#include <experimental/optional>

#include <cat/functor/optional.hpp>
#include <cat/applicative/applicative.hpp>

namespace cat
{
    // experimental::optional is an applicative instance:
    //

    template <> struct is_applicative<std::experimental::optional> : std::true_type { };

    // experimental::optional instance:
    //

    template <typename Fun, typename A, typename ...Ts>
    struct ApplicativeInstance<std::experimental::optional, Fun, A, Ts...> : Applicative<std::experimental::optional>::Class<Fun, A, Ts...>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        std::experimental::optional<A>
        pure(A const &elem)
        {
            return std::experimental::make_optional(elem);
        }

        std::experimental::optional<B>
        apply(std::experimental::optional<Fun> const &f, std::experimental::optional<A> const &x) final
        {
            if (f && x)
                return std::experimental::make_optional<B>((*f)(*x));

            return std::experimental::optional<B>();
        }
    };

} // namespace cat

