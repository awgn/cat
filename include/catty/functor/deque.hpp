#pragma once

#include <deque>

#include <catty/functor/functor.hpp>


namespace catty
{
    // deque instance:
    //

    template <typename Fun, typename A>
    auto fmap(Fun fun, std::deque<A> const &xs)
    {
        std::deque< decltype(fun( xs.front())) > out;

        for(auto & x : xs)
            out.push_back(fun(x));

        return out;
    }

    template <>
    struct typeclass_instance2<Functor, std::deque>
    {
        using type =
            typeclass
            <
                SYMBOL(fmap, decltype(fmap(_f, std::deque<_A>{})))
            >;
    };

} // namespace catty

