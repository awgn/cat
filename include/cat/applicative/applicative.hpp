#pragma once

#include <utility>

#include <cat/functor/functor.hpp>

namespace cat
{
    // Class Applicative:
    //

    template <template <typename...> class F>
    struct Applicative
    {
        template <typename Fun, typename A>
        struct Class
        {
            virtual auto pure(A const &) const -> F<A> = 0; // lift a value.
            virtual auto apply(F<Fun> const &, F<A> const &) const -> F<decltype(std::declval<Fun>()(std::declval<A>()))> = 0;
        };
    };


    template <template <typename ...> class F, typename ...> struct ApplicativeInstance;

    template <template <typename ...> class F, typename A>
    auto pure(A const &value)
    {
        return ApplicativeInstance<F, id, A>{}.pure(value);
    }

    template <template <typename ...> class F, typename Fun, typename A>
    auto apply(F<Fun> const &fs, F<A> const &xs)
    {
        return ApplicativeInstance<F, Fun, A>{}.apply(fs, xs);
    }


    template <template <typename ...> class A>
    struct is_applicative : std::false_type
    { };


} // namespace cat

