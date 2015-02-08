#pragma once

#include <list>

#include <catty/functor/functor.hpp>

namespace catty
{
    // list instance:
    //

    template <typename A, typename Fun >
    auto fmap(Fun fun, std::list<A> const &xs)
    {
        std::list< decltype(fun( xs.front() )) > out;

        for(auto & x : xs)
        {
            out.push_back(fun(x));
        }

        return out;
    }

    template <>
    struct typeclass_instance2<Functor, std::list>
    {
        using type =
            typeclass
            <
                OVERLOADED_FUNCTION(fmap, F_<a_,b_>, std::list<a_> const &, std::list<b_> )
            >;
    };

} // namespace catty

