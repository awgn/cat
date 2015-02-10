#pragma once

#include <list>
#include <cat/monoid/monoid.hpp>

namespace cat
{
    template <typename T>
    struct is_monoid<std::list<T>> : std::true_type { };


    template <typename T, template <typename ...> class F>
    struct MonoidInstance<std::list<T>, F> : Monoid<std::list<T>>::template Class<F>
    {
        virtual std::list<T> mempty() final
        {
            return std::list<T>{};
        }

        virtual std::list<T> mappend(std::list<T> const &a, std::list<T> const &b) final
        {
            auto ret = a;

            for(auto const &x : b)
                ret.push_back(x);

            return ret;
        }
    };

};
