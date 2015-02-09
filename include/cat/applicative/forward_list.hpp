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

    template <typename Fun, typename A>
    struct ApplicativeInstance<std::forward_list, Fun, A> : Applicative<std::forward_list>::Class<Fun, A>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        auto pure(A const &elem) const -> std::forward_list<A> final
        {
            return std::forward_list<A>{ elem };
        }

        auto apply(std::forward_list<Fun> const &fs, std::forward_list<A> const &xs) const -> std::forward_list<B> final
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

