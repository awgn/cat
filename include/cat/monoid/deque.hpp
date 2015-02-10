#pragma once

#include <deque>
#include <cat/monoid/monoid.hpp>

namespace cat
{
    template <typename T>
    struct is_monoid<std::deque<T>> : std::true_type { };


    template <typename T, template <typename ...> class F>
    struct MonoidInstance<std::deque<T>, F> : Monoid<std::deque<T>>::template Class<F>
    {
        virtual std::deque<T> mempty() final
        {
            return std::deque<T>{};
        }

        virtual std::deque<T> mappend(std::deque<T> const &a, std::deque<T> const &b) final
        {
            auto ret = a;

            for(auto const &x : b)
                ret.push_back(x);

            return ret;
        }
    };

};
