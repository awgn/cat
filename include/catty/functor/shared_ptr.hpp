#pragma once

#include <memory>

#include <catty/functor/functor.hpp>

namespace catty
{
    // shared_ptr instance:
    //

    template <typename A, typename Fun>
    auto fmap(Fun fun, std::shared_ptr<A> const &xs)
    {
        using type = decltype(fun(*xs));

        if (xs)
            return std::make_shared<type>(fun(*xs));

        return std::shared_ptr<type>();
    }

    template <>
    struct typeclass_instance2<Functor, std::shared_ptr>
    {
        using type =
            typeclass
            <
                SYMBOL(fmap, decltype(fmap(_f, std::shared_ptr<_A>{})))
            >;
    };

} // namespace catty

