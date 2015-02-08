#pragma once

#include <memory>

#include <catty/functor/functor.hpp>

namespace catty
{
    // unique_ptr instance:
    //

    template <typename A, typename Fun>
    auto fmap(Fun fun, std::unique_ptr<A> const &xs)
    {
        using type = decltype(fun(*xs));

        if (xs)
            return std::make_unique<type>(fun(*xs));

        return std::unique_ptr<type>();
    }

    template <>
    struct typeclass_instance2<Functor, std::unique_ptr>
    {
        using type =
            typeclass
            <
                SYMBOL(fmap, decltype(fmap(_f, std::unique_ptr<_A>{})))
            >;
    };

} // namespace catty

