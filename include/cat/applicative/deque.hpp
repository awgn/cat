#pragma once

#include <deque>

#include <cat/functor/deque.hpp>
#include <cat/applicative/applicative.hpp>

namespace cat
{
    // deque is an applicative instance:
    //

    template <> struct is_applicative<std::deque> : std::true_type { };

    // deque instance:
    //

    template <typename Fun, typename A, typename ...Ts>
    struct ApplicativeInstance<std::deque, Fun, A, Ts...> : Applicative<std::deque>::Class<Fun, A, Ts...>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        std::deque<A>
        pure(A const &elem) final
        {
            return std::deque<A>{ elem };
        }

        std::deque<B>
        apply(std::deque<Fun> const &fs, std::deque<A, Ts...> const &xs) final
        {
            std::deque<B> out;

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

