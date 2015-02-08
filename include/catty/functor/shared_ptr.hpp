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
                OVERLOADED_FUNCTION(fmap, F_<a_,b_>, std::shared_ptr<a_> const &, std::shared_ptr<b_> )
            >;
    };

} // namespace catty

