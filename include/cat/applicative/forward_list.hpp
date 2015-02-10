#pragma once

#include <forward_list>

#include <cat/functor/forward_list.hpp>
#include <cat/applicative/applicative.hpp>

namespace cat
{
    // forward_list is an applicative instance:
    //

    template <> struct is_applicative<std::forward_list> : std::true_type { };

    // forward_list instance:
    //

    template <typename Fun, typename A, typename ...Ts>
    struct ApplicativeInstance<std::forward_list, Fun, A, Ts...> : Applicative<std::forward_list>::Class<Fun, A, Ts...>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        std::forward_list<A>
        pure(A const &elem) final
        {
            return std::forward_list<A>{ elem };
        }

        std::forward_list<B>
        apply(std::forward_list<Fun> const &fs, std::forward_list<A> const &xs) final
        {
            std::forward_list<B> out;

            for(auto const &f : fs)
            {
                for(auto const &x : xs)
                {
                    out.push_front(f(x));
                }
            }

            out.reverse();
            return out;
        }
    };

} // namespace cat

