#pragma once

#include <forward_list>
#include <cat/monoid/monoid.hpp>

namespace cat
{
    template <typename T>
    struct is_monoid<std::forward_list<T>> : std::true_type { };


    template <typename T, template <typename ...> class F>
    struct MonoidInstance<std::forward_list<T>, F> : Monoid<std::forward_list<T>>::template Class<F>
    {
        virtual std::forward_list<T> mempty() final
        {
            return std::forward_list<T>{};
        }

        virtual std::forward_list<T> mappend(std::forward_list<T> const &a, std::forward_list<T> const &b) final
        {
            auto ret = a;
            ret.reverse();

            for(auto const &x : b)
                ret.push_front(x);

            ret.reverse();
            return ret;
        }
    };

};
