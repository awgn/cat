#pragma once

#include <deque>

#include <catty/functor/deque.hpp>
#include <catty/applicative/applicative.hpp>

namespace catty
{
    // deque instance:
    //

    template <typename A>
    auto pure(A const &elem, tag<std::deque>)
    {
        return std::deque<A>{ elem };
    }

    template <typename Fun, typename A>
    auto apply(std::deque<Fun> const &fs, std::deque<A> const &xs)
    {
        std::deque< decltype( std::declval<Fun>()( xs.front() )) > out;

        for(auto &f : fs)
        {
            for(auto &x : xs)
            {
                out.push_back(f(x));
            }
        }

        return out;
    }

    template <>
    struct typeclass_instance2<Applicative, std::deque>
    {
        using type =
            typeclass
            <
                OVERLOADED_FUNCTION(pure,  a_ const&, std::deque<a_> ),
                OVERLOADED_FUNCTION(apply, std::deque<F_<a_,b_>> const &, std::deque<a_> const&, std::deque<b_> )
            >;
    };

} // namespace catty

