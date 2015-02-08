#pragma once

#include <map>

#include <catty/functor/functor.hpp>

namespace catty
{
    // map instance:
    //

    template <typename K, typename A, typename Fun>
    auto fmap(Fun fun, std::map<K, A> const &xs)
    {
        std::map<K, decltype(fun( std::declval<A>() )) > out;

        for(auto & x : xs)
        {
            out.insert(std::make_pair(x.first, fun(x.second)));
        }

        return out;
    }

    template <>
    struct typeclass_instance2<Functor, std::map>
    {
        using type =
            typeclass
            <
                OVERLOADED_FUNCTION(fmap, F_<a_,b_>, std::map<k_, a_> const &, std::map<k_, b_> )
            >;
    };

} // namespace catty

