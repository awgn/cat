#pragma once

#include <experimental/optional>

#include <cat/functor/optional.hpp>
#include <cat/applicative/applicative.hpp>

namespace cat
{
    // optional instance:
    //

    template <typename A>
    auto pure(A const &elem, tag<std::experimental::optional>)
    {
        return std::experimental::make_optional(elem);
    }

    template <typename Fun, typename A>
    auto apply(std::experimental::optional<Fun> const &f, std::experimental::optional<A> const &x)
    {
        using type = decltype(std::declval<Fun>()(std::declval<A>()));

        if (f && x) {
            return std::experimental::make_optional<type>((*f)(*x));
        }

        return std::experimental::optional<type>();
    }

    template <>
    struct typeclass_instance2<Applicative, std::experimental::optional>
    {
        using type =
            typeclass
            <
                OVERLOADED_FUNCTION(pure,  a_ const&, std::experimental::optional<a_> ),
                OVERLOADED_FUNCTION(apply, std::experimental::optional<F_<a_,b_>> const &, std::experimental::optional<a_> const&, std::experimental::optional<b_> )
            >;
    };

} // namespace cat

