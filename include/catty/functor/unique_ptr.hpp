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
                OVERLOADED_FUNCTION(fmap, F_<a_,b_>, std::unique_ptr<a_> const &, std::unique_ptr<b_> )
            >;
    };

} // namespace catty

