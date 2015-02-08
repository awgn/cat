#pragma once

#include <list>

#include <catty/functor/functor.hpp>

namespace catty
{
    // list instance:
    //

    template <typename Fun, typename A>
    auto fmap(Fun fun, std::list<A> const &xs)
    {
        std::list< decltype(fun( xs.front() )) > out;

        for(auto & x : xs)
            out.push_back(fun(x));

        return out;
    }

    template <>
    struct typeclass_instance2<Functor, std::list>
    {
        using type =
            typeclass
            <
                SYMBOL(fmap, decltype(fmap(_f, std::list<_A>{})))
            >;
    };

} // namespace catty

