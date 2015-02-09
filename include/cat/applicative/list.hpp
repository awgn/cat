#pragma once

#include <list>

#include <cat/functor/list.hpp>
#include <cat/applicative/applicative.hpp>

namespace cat
{
    // list is an applicative instance:

    template <> struct is_applicative<std::list> : std::true_type { };

    // list instance:
    //

    template <typename Fun, typename A>
    struct ApplicativeInstance<std::list, Fun, A> : Applicative<std::list>::Class<Fun, A>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        auto pure(A const &elem) const -> std::list<A> final
        {
            return std::list<A>{ elem };
        }

        auto apply(std::list<Fun> const &fs, std::list<A> const &xs) const -> std::list<B> final
        {
            std::list<B> out;

            for(auto const &f : fs)
            {
                for(auto const &x : xs)
                {
                    out.push_back(f(x));
                }
            }

            return out;
        }
    };

} // namespace cat

