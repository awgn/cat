#pragma once

#include <vector>

#include <catty/functor/vector.hpp>
#include <catty/applicative/applicative.hpp>

namespace catty
{
    // vector instance:
    //

    template <typename A>
    auto pure(A const &elem, tag<std::vector>)
    {
        return std::vector<A>{ elem };
    }

    template <typename Fun, typename A>
    auto apply(std::vector<Fun> const &fs, std::vector<A> const &xs)
    {
        std::vector< decltype( std::declval<Fun>()( xs.front() )) > out;
        out.reserve(fs.size() * xs.size());

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
    struct typeclass_instance2<Applicative, std::vector>
    {
        using type =
            typeclass
            <
                OVERLOADED_FUNCTION(pure,  a_ const&, std::vector<a_> ),
                OVERLOADED_FUNCTION(apply, std::vector<F_<a_,b_>> const &, std::vector<a_> const&, std::vector<b_> )
            >;
    };

} // namespace catty

