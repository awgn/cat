#pragma once

#include <utility>

#include <catty/typeclass.hpp>


namespace catty
{
    //
    // typeclass Functor:
    //

    template <template <typename...> class F>
    struct Functor
    {
        static auto fmap(_<_A, _B> f, F<_A> const&) -> F <decltype(f(std::declval<_A>()))>;

        using type = typeclass
        <
            SYMBOL(fmap, decltype(fmap(_f, std::declval<F<_A>>())))
        >;
    };

    //
    // constraint:
    //

    template <template <typename...> class F>
    constexpr bool FunctorInstance()
    {
        return equal_set<typename Functor<F>::type, typename typeclass_instance2<Functor, F>::type>();
    };


} // namespace catty

