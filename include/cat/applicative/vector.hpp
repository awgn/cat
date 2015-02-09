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

    template <typename Fun, typename A>
    struct ApplicativeInstance<std::vector, Fun, A> : Applicative<std::vector>::Class<Fun, A>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        auto pure(A const &elem) const -> std::vector<A> final
        {
            return std::vector<A>{ elem };
        }

        auto apply(std::vector<Fun> const &fs, std::vector<A> const &xs) const -> std::vector<B> final
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

