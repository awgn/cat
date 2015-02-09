#pragma once

#include <utility>

#include <cat/typeclass.hpp>
#include <cat/functor/functor.hpp>

namespace cat
{
    // class Applicative:
    //

    template <template <typename...> class F>
    struct Applicative
    {
        template <typename A>
        static auto pure(A const &) -> F<A>; // lift a value.

        template <typename Fun, typename A>
        static auto apply( F<Fun> const &, F<A> const &) -> F< decltype(std::declval<Fun>()(std::declval<A>())) >;

        using type = typeclass
        <
            OVERLOADED_SYMBOL(pure,  decltype(pure<a_>)),
            OVERLOADED_SYMBOL(apply, decltype(apply<F_<a_,b_>, a_>))
        >;
    };

    // constraint:
    //

    template <template <typename...> class F>
    constexpr bool ApplicativeInstance()
    {
        return FunctorInstance<F>() &&
                 equal_set<typename typeclass_instance2<Applicative,F>::type, typename Applicative<F>::type>();
    };


    template <template <typename ...> class F, typename A>
    auto pure(A const &elem)
    {
        return pure(elem, tag<F>{});
    }

} // namespace cat

