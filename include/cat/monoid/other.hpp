#pragma once

#include <cat/monoid/monoid.hpp>

namespace cat
{
    struct Any
    {
        bool value;
        explicit operator bool() const { return value; }
    };

    struct All
    {
        bool value;
        explicit operator bool() const { return value; }
    };

    inline bool operator==(Any a, Any b) { return a.value == b.value; }
    inline bool operator!=(Any a, Any b) { return !(a == b); }
    inline bool operator==(All a, All b) { return a.value == b.value; }
    inline bool operator!=(All a, All b) { return !(a == b); }

    template <>
    struct is_monoid<Any> : std::true_type { };

    template <>
    struct is_monoid<All> : std::true_type { };

    template <template <typename ...> class F>
    struct MonoidInstance<Any, F> : Monoid<Any>::template Class<F>
    {
        virtual Any mempty() final
        {
            return Any{ false };
        }

        virtual Any mappend(Any const &a, Any const &b) final
        {
            return Any { a.value || b.value };
        }
    };

    template <template <typename ...> class F>
    struct MonoidInstance<All, F> : Monoid<All>::template Class<F>
    {
        virtual All mempty() final
        {
            return All{ true };
        }

        virtual All mappend(All const &a, All const &b) final
        {
            return All { a.value && b.value };
        }
    };

};
