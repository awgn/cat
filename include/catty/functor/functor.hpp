#pragma once

#include <utility>

#include <catty/typeclass.hpp>


namespace catty
{
    // class Functor:
    //

    template <template <typename...> class F>
    struct Functor
    {
        template <typename Fun, typename A>
        static auto fmap(Fun fun, F<A> const&) -> F<decltype(fun(std::declval<A>()))>;

        using type = typeclass
        <
        >;
    };

    // constraint:
    //

    template <template <typename...> class F>
    constexpr bool FunctorInstance()
    {
        return equal_set<typename typeclass_instance2<Functor,F>::type, typename Functor<F>::type>();
    };


} // namespace catty

