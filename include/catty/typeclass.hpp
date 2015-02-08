#pragma once

#include <string>
#include <functional>

#include "bits/typelist.hpp"
#include "bits/typeset.hpp"


#define SYMBOL(fun, type) \
    tsymbol<decltype(#fun ## _tstr), type>


namespace catty
{
    template <typename, typename> struct to_function;
    template <typename R, typename ...Ts>
    struct to_function<R, pack<Ts...>>
    {
        using type = std::function<R(Ts...)>;
    };

    // curried function: _<A,B> = std::function<B(A)>
    //

    template <typename ...Ts>
    using _ = typename to_function<last_t<Ts...>, init_t<Ts...>>::type;

    namespace
    {
        struct _A { } _a = {};
        struct _B { } _b = {};
        struct _C { } _c = {};
        struct _D { } _d = {};

        auto _f = _<_A, _B>{};
    }

    /// type type

    template <template <typename ...> class F>
    struct tag { };


    /// template string (C++14 required):

    template <char ...cs>
    struct tstring { };

    template <typename CharT, CharT ...str>
    tstring<str...> operator""_tstr()
    {
        return tstring<str...>{};
    }

    /// template symbol:

    template <typename Tstring, typename Fun> struct tsymbol { };


    /// typeclass:

    template <typename ...Fs>
    using typeclass = multiset<Fs...>;


    /// typeclass_instance:

    template <template <typename> class C, typename T> struct typeclass_instance
    {
        using type = typeclass<>;
    };

    template <template <template <typename...> class> class C, template <typename...> class F> struct typeclass_instance2
    {
        using type = typeclass<>;
    };

} // namespace catty
