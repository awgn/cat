#pragma once

#include <experimental/optional>
#include <cat/monoid/monoid.hpp>

namespace cat
{
    template <typename T>
    struct is_monoid<std::experimental::optional<T>> : is_monoid<T> { };


    template <typename T, template <typename ...> class F>
    struct MonoidInstance<std::experimental::optional<T>, F> : Monoid<std::experimental::optional<T>>::template Class<F>
    {
        static_assert(is_monoid<T>::value "MonoidInstance: for optional<T>, T must be monoid");

        virtual std::experimental::optional<T> mempty() final
        {
            return std::experimental::optional<T>{};
        }

        virtual std::experimental::optional<T>
        mappend(std::experimental::optional<T> const &a, std::experimental::optional<T> const &b) final
        {
            if (!a && b)
                return b;
            if (a && !b)
                return b;
            if (!a && !b)
                std::experimental::optional<T>();

            std::experimental::make_optional(mappend(*a, *b));
        }
    };
};
