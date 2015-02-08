#pragma once

namespace catty
{
    template <typename ...Ts> struct pack {};

    template <typename P1, typename P2> struct cons;


    template <typename ...Ts, typename ...Vs>
    struct cons<pack<Ts...>, pack<Vs...>>
    {
        using type = pack<Ts..., Vs...>;
    };

    template <typename P1, typename P2>
    using cons_t = typename cons<P1,P2>::type;

    template <typename ...Ts> struct last;

    template <typename T>
    struct last<T>
    {
        using type = T;
    };

    template <typename T, typename ...Ts>
    struct last<T, Ts...>
    {
        using type = typename last<Ts...>::type;
    };

    template <typename ...Ts>
    using last_t = typename last<Ts...>::type;

    template <typename ...Ts> struct init;

    template <typename T>
    struct init<T>
    {
        using type = pack<>;
    };

    template <typename T, typename ...Ts>
    struct init<T, Ts...>
    {
        using type = cons_t<pack<T>, typename init<Ts...>::type>;
    };

    template <typename ...Ts>
    using init_t = typename init<Ts...>::type;

    template <typename ...Ts> struct head;
    template <typename T, typename ...Ts>
    struct head<T, Ts...>
    {
        using type = T;
    };

    template <typename ...Ts>
    using head_t = typename head<Ts...>::type;

    template <typename ...Ts> struct tail;
    template <typename T, typename ...Ts>
    struct tail<T, Ts...>
    {
        using type = pack<Ts...>;
    };

    template <typename ...Ts>
    using tail_t = typename tail<Ts...>::type;

} // namespace catty

