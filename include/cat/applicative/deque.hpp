#pragma once

#include <deque>

#include <cat/functor/deque.hpp>
#include <cat/applicative/applicative.hpp>

namespace cat
{
    // deque is an applicative instance:

    template <> struct is_applicative<std::deque> : std::true_type { };

    // deque instance:
    //

    template <typename Fun, typename A>
    struct ApplicativeInstance<std::deque, Fun, A> : Applicative<std::deque>::Class<Fun, A>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        auto pure(A const &elem) const -> std::deque<A> final
        {
            return std::deque<A>{ elem };
        }

        auto apply(std::deque<Fun> const &fs, std::deque<A> const &xs) const -> std::deque<B> final
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

