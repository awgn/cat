#pragma once

#include <experimental/optional>

#include <cat/functor/optional.hpp>
#include <cat/applicative/applicative.hpp>

namespace cat
{
    // experimental::optional is an applicative instance:

    template <> struct is_applicative<std::experimental::optional> : std::true_type { };

    // experimental::optional instance:
    //

    template <typename Fun, typename A>
    struct ApplicativeInstance<std::experimental::optional, Fun, A> : Applicative<std::experimental::optional>::Class<Fun, A>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        auto pure(A const &elem) const -> std::experimental::optional<A> final
        {
            return std::experimental::make_optional(elem);
        }

        auto apply(std::experimental::optional<Fun> const &f, std::experimental::optional<A> const &x) const
                -> std::experimental::optional<B> final
        {
            if (f && x)
                return std::experimental::make_optional<B>((*f)(*x));

            return std::experimental::optional<B>();
        }
    };

} // namespace cat

