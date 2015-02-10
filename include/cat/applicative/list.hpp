#pragma once

#include <list>

#include <cat/functor/list.hpp>
#include <cat/applicative/applicative.hpp>

namespace cat
{
    // list is an applicative instance:
    //

    template <> struct is_applicative<std::list> : std::true_type { };

    // list instance:
    //

    template <typename Fun, typename A, typename ...Ts>
    struct ApplicativeInstance<std::list, Fun, A, Ts...> : Applicative<std::list>::Class<Fun, A, Ts...>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        std::list<A>
        pure(A const &elem) final
        {
            return std::list<A>{ elem };
        }

        std::list<B>
        apply(std::list<Fun> const &fs, std::list<A> const &xs) final
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

