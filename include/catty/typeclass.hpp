#pragma once

#include <string>
#include <functional>

#include "bits/typeset.hpp"
#include "bits/function.hpp"


#define FUN_DECLTYPE(fun) \
template<typename... Ts> \
using fun ## Type = decltype(fun(std::declval<Ts>()...))(*)(Ts...);

#define SYMBOL(fun)                         tsymbol<decltype(#fun ## _tstr), decltype(fun)>
#define OVERLOADED_SYMBOL(fun, typ...)      tsymbol<decltype(#fun ## _tstr), typ>
#define OVERLOADED_FUNCTION(fun, args...)   tsymbol<decltype(#fun ## _tstr), F_<args>>


namespace catty
{
    /// type type

    template <template <typename ...> class F>
    struct tag { };

    /// template string (c++14 required):

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
