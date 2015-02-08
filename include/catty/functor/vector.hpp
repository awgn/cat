#pragma once

#include <vector>

#include <catty/functor/functor.hpp>

namespace catty
{
    // vector instance:
    //

    template <typename A, typename Fun>
    auto fmap(Fun fun, std::vector<A> const &xs)
    {
        std::vector< decltype(fun( xs.front() )) > out;
        out.reserve(xs.size());

        for(auto & x : xs)
        {
            out.push_back(fun(x));
        }

        return out;
    }

    //
    // template <>
    // struct typeclass_instance2<Functor, std::vector>
    // {
    //     using type =
    //         typeclass
    //         <
    //             OVERLOADED_FUNCTION(fmap, F_<a_,b_>, std::vector<a_> const &, std::vector<b_> )
    //         >;
    // };

} // namespace catty

