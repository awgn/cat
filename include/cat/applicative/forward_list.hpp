#pragma once

#include <forward_list>

#include <cat/functor/forward_list.hpp>
#include <cat/applicative/applicative.hpp>

namespace cat
{
    // forward_list instance:
    //

    template <typename A>
    auto pure(A const &elem, tag<std::forward_list>)
    {
        return std::forward_list<A>{ elem };
    }

    template <typename Fun, typename A>
    auto apply(std::forward_list<Fun> const &fs, std::forward_list<A> const &xs)
    {
        std::forward_list< decltype( std::declval<Fun>()( xs.front() )) > out;

        for(auto &f : fs)
        {
            for(auto &x : xs)
            {
                out.push_front(f(x));
            }
        }

        out.reverse();
        return out;
    }

    template <>
    struct typeclass_instance2<Applicative, std::forward_list>
    {
        using type =
            typeclass
            <
                OVERLOADED_FUNCTION(pure,  a_ const&, std::forward_list<a_> ),
                OVERLOADED_FUNCTION(apply, std::forward_list<F_<a_,b_>> const &, std::forward_list<a_> const&, std::forward_list<b_> )
            >;
    };

} // namespace cat

