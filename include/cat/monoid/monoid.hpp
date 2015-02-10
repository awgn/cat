#pragma once

#include <cat/bits/utility.hpp>
#include <vector>

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
            virtual M mempty() = 0;
            virtual M mappend(M const &, M const &) = 0;

            virtual M mconcat(F<M> const &xs)
            {
                auto r = mempty();
                for(auto const & x : xs)
                    r = mappend(r, x);
                return r;
            };
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

