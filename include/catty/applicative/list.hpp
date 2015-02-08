#pragma once

#include <list>

#include <catty/functor/list.hpp>
#include <catty/applicative/applicative.hpp>

namespace catty
{
    // list instance:
    //

    template <typename A>
    auto pure(A const &elem, tag<std::list>)
    {
        return std::list<A>{ elem };
    }

    template <typename Fun, typename A>
    auto apply(std::list<Fun> const &fs, std::list<A> const &xs)
    {
        std::list< decltype( std::declval<Fun>()( xs.front() )) > out;

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
    struct typeclass_instance2<Applicative, std::list>
    {
        using type =
            typeclass
            <
                OVERLOADED_FUNCTION(pure,  a_ const&, std::list<a_> ),
                OVERLOADED_FUNCTION(apply, std::list<F_<a_,b_>> const &, std::list<a_> const&, std::list<b_> )
            >;
    };

} // namespace catty

