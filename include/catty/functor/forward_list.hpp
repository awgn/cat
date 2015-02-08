#pragma once

#include <forward_list>

#include <catty/functor/functor.hpp>


namespace catty
{
    // forward_list instance:
    //

    template <typename A, typename Fun>
    auto fmap(Fun fun, std::forward_list<A> const &xs)
    {
        std::forward_list<decltype(fun(std::declval<A>()))> out;

        for(auto const & x : xs)
            out.push_front(fun(x));

        out.reverse();
        return out;
    }

    template <>
    struct typeclass_instance2<Functor, std::forward_list>
    {
        using type =
            typeclass
            <
                SYMBOL(fmap, decltype(fmap(_f, std::forward_list<_A>{})))
            >;
    };

} // namespace catty

