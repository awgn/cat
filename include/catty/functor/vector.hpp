#pragma once

#include <vector>

#include <catty/functor/functor.hpp>

namespace catty
{
    // vector instance:
    //

    template <typename Fun, typename A>
    auto fmap(Fun fun, std::vector<A> const &xs)
    {
        std::vector< decltype(fun( xs.front() )) > out;
        out.reserve(xs.size());

        for(auto & x : xs)
            out.push_back(fun(x));

        return out;
    }


    template <>
    struct typeclass_instance2<Functor, std::vector>
    {
        using type =
        typeclass
        <
            SYMBOL(fmap, decltype(fmap(_f, std::vector<_A>{})))
        >;
    };

} // namespace catty

