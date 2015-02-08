#pragma once

#include <experimental/optional>

#include <catty/functor/functor.hpp>

namespace catty
{
    // experimental::optional instance:
    //

    template <typename A, typename Fun>
    auto fmap(Fun fun, std::experimental::optional<A> const &xs)
    {
        using type = decltype(fun(*xs));

        if (xs)
            return std::experimental::make_optional<type>(fun(*xs));

        return std::experimental::optional<type>();
    }

    template <>
    struct typeclass_instance2<Functor, std::experimental::optional>
    {
        using type =
            typeclass
            <
                OVERLOADED_FUNCTION(fmap, F_<a_,b_>, std::experimental::optional<a_> const &, std::experimental::optional<b_> )
            >;
    };

} // namespace catty

