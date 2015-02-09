#pragma once

#include <cat/bits/utility.hpp>

namespace cat
{
    //
    // class Monoid:
    //

    template <typename M>
    struct Monoid
    {
        template <template <typename ...> class F>
        struct Class
        {
            virtual M mempty() const = 0; // identity
            virtual M mappend(M const &, M const &) const = 0; // identity
            virtual M mconcat(F<M> const &) const = 0; // identity
        };
    };


    template <typename M, template <typename ...> class> struct MonoidInstance;

    template <typename M>
    M mempty()
    {
        return MonoidInstance<M, std::vector>{}.mempty();
    }

    template <typename M>
    M mappend(M const &a, M const &b)
    {
        return MonoidInstance<M, std::vector>{}.mappend(a, b);
    }

    template <template <typename ...> class F, typename M>
    M mconcat(F<M> const &f)
    {
        return MonoidInstance<M, F>{}.mconcat(f);
    }


    template <typename M>
    struct is_monoid : std::false_type
    { };


} // namespace cat

