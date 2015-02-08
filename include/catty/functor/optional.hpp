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
                SYMBOL(fmap, decltype(fmap(_f, std::experimental::optional<_A>{})))
            >;
    };

} // namespace catty

