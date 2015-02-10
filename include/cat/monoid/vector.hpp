#pragma once

#include <vector>
#include <cat/monoid/monoid.hpp>

namespace cat
{
    template <typename T>
    struct is_monoid<std::vector<T>> : std::true_type { };


    template <typename T, template <typename ...> class F>
    struct MonoidInstance<std::vector<T>, F> : Monoid<std::vector<T>>::template Class<F>
    {
        virtual std::vector<T> mempty() final
        {
            return std::vector<T>{};
        }

        virtual std::vector<T> mappend(std::vector<T> const &a, std::vector<T> const &b) final
        {
            auto ret = a;
            ret.reserve(a.size() + b.size());
            for(auto const &x : b)
                ret.push_back(x);

            return ret;
        }
    };
};
