#pragma once

#include <deque>

#include <catty/functor/functor.hpp>


namespace catty
{
    // deque instance:
    //

    template <typename A, typename Fun>
    auto fmap(Fun fun, std::deque<A> const &xs)
    {
        std::deque< decltype(fun( xs.front() )) > out;

        for(auto & x : xs)
        {
            out.push_back(fun(x));
        }

        return out;
    }

    template <>
    struct typeclass_instance2<Functor, std::deque>
    {
        using type =
            typeclass
            <
                OVERLOADED_FUNCTION(fmap, F_<a_,b_>, std::deque<a_> const &, std::deque<b_> )
            >;
    };

} // namespace catty

