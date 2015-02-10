#pragma once

#include <utility>

#include <cat/functor/functor.hpp>

namespace cat
{
    //
    // class Applicative:
    //

    template <template <typename...> class F>
    struct Applicative
    {
        template <typename Fun, typename A, typename ...Ts>
        struct Class
        {
            virtual auto pure(A const &) -> F<A, Ts...> = 0; // lift a value.
            virtual auto apply(F<Fun> const &, F<A, Ts...> const &) -> F<decltype(std::declval<Fun>()(std::declval<A>()))>
            { throw nullptr; }
        };
    };


    template <template <typename ...> class F, typename ...> struct ApplicativeInstance;

    template <template <typename ...> class F, typename A>
    auto pure(A const &value)
    {
        return ApplicativeInstance<F, Id, A>{}.pure(value);
    }

    template <template <typename ...> class F, typename Fun, typename A, typename ...Ts>
    auto apply(F<Fun> const &fs, F<A, Ts...> const &xs)
    {
        return ApplicativeInstance<F, Fun, A, Ts...>{}.apply(fs, xs);
    }


    template <template <typename ...> class F, typename Fun, typename A, typename ...Ts>
    auto operator*(F<Fun> const &fs, F<A, Ts...> const &xs)
    {
        return apply(fs, xs);
    }

    template <template <typename ...> class A>
    struct is_applicative : std::false_type
    { };


} // namespace cat

