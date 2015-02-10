#pragma once

#include <vector>

#include <cat/functor/vector.hpp>
#include <cat/applicative/applicative.hpp>

namespace cat
{
    // vector is an applicative instance:
    //

    template <> struct is_applicative<std::vector> : std::true_type { };

    // vector instance:
    //

    template <typename Fun, typename A, typename ...Ts>
    struct ApplicativeInstance<std::vector, Fun, A, Ts...> : Applicative<std::vector>::Class<Fun, A, Ts...>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        std::vector<A>
        pure(A const &elem) final
        {
            return std::vector<A>{ elem };
        }

        std::vector<B>
        apply(std::vector<Fun> const &fs, std::vector<A, Ts...> const &xs) final
        {
            std::vector<B> out;
            out.reserve(fs.size() * xs.size());

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

