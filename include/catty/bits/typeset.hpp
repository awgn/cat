#pragma once

#include <type_traits>

namespace catty {

    // multiset
    //

    template <typename ...Ts> struct multiset
    {
        static constexpr size_t size = sizeof...(Ts);
    };


    // concat
    //

    template <typename Set1, typename Set2> struct concat;
    template <typename Set1, typename Set2>
    using concat_t = typename concat<Set1, Set2>::type;

    template <typename ...Ts, typename ...Vs>
    struct concat<multiset<Ts...>, multiset<Vs...>>
    {
        using type = multiset<Ts..., Vs...>;
    };

    // erase a type from the multiset
    //

    template <typename T, typename Set> struct erase;
    template <typename T, typename Set>
    using erase_t = typename erase<T, Set>::type;

    template <typename T, typename V, typename ...Ts>
    struct erase<V, multiset<T, Ts...>>
    {
        using type = concat_t<multiset<T>, erase_t<V, multiset<Ts...>>>;
    };

    template <typename T, typename ...Ts>
    struct erase<T, multiset<T, Ts...>>
    {
        using type = multiset<Ts...>;
    };

    template <typename T>
    struct erase<T, multiset<>>
    {
        using type = multiset<>;
    };

    // subtract a multiset from a multiset
    //

    template <typename Set1, typename Set2> struct subtract;
    template <typename Set1, typename Set2>
    using subtract_t = typename subtract<Set1, Set2>::type;

    template <typename ...Ts>
    struct subtract< multiset<Ts...>, multiset<> >
    {
        using type = multiset<Ts...>;
    };

    template <typename V, typename ...Ts, typename ...Vs>
    struct subtract< multiset<Ts...>, multiset<V, Vs...> >
    {
        using type = subtract_t< erase_t<V, multiset<Ts...>>, multiset<Vs...> >;
    };

    // equal_set
    //

    template <typename Set1, typename Set2>
    constexpr bool equal_set()
    {
        return (Set1::size == Set2::size) && std::is_same<subtract_t<Set1, Set2>, multiset<> >::value;
    }

} // namespace type

