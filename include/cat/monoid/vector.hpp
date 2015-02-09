#pragma once

#include <vector>
#include <cat/monoid/monoid.hpp>

namespace cat
{
    template <typename T>
    struct is_monoid<std::vector<T>> : std::true_type { };


    template <typename T, template <typename ...> class F>
    struct MonoidInstance<std::vector<T>, F> : Monoid<std::vector<T>>
    {
        virtual std::vector<T> mempty() const final
        {
            return std::vector<T>{};
        }

        virtual std::vector<T> mappend(std::vector<T> const &a, std::vector<T> const &b) const
        {
            auto ret = a;
            for(auto const &x : b)
            {
                ret.push_back(x);
            }
            return ret;
        }

        virtual std::vector<T> mconcat(F<std::vector<T>> const &xs) const
        {
            auto r = mempty();

            for(auto const & x : xs)
            {
                r = mappend(r, x);
            }

            return r;
        }
    };

};
