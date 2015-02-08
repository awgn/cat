#pragma once

#include <unordered_map>

#include <catty/functor/functor.hpp>

namespace catty
{
    // unordered_map instance:
    //

    template <typename K, typename A, typename Fun>
    auto fmap(Fun fun, std::unordered_map<K, A> const &xs)
    {
        std::unordered_map<K, decltype(fun( std::declval<A>() )) > out;

        for(auto & x : xs)
        {
            out.insert(std::make_pair(x.first, fun(x.second)));
        }

        return out;
    }

    template <>
    struct typeclass_instance2<Functor, std::unordered_map>
    {
        using type =
            typeclass
            <
                OVERLOADED_FUNCTION(fmap, F_<a_,b_>, std::unordered_map<k_, a_> const &, std::unordered_map<k_, b_> )
            >;
    };

} // namespace catty

