#pragma once

#include <functional>

#include "typelist.hpp"

namespace catty
{
    template <typename R, typename Pack> struct to_function;
    template <typename R, typename ...Ts>
    struct to_function<R, pack<Ts...>>
    {
        using type = R(Ts...);
    };

    // generic placeholder types:
    //

    struct a_ { };
    struct b_ { };
    struct c_ { };
    struct k_ { };

    // curried function:
    //

    template <typename ...Ts>
    using F_ = typename to_function<last_t<Ts...>, init_t<Ts...>>::type;


} // namespace catty

