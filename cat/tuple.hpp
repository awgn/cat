/******************************************************************************
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014-15 Nicola Bonelli <nicola@pfq.io>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 ******************************************************************************/

#pragma once

#include <cat/__config.hpp>

#include <tuple>
#include <utility>

namespace cat
{
    //
    // implementation details...
    //

    namespace details
    {
        template <size_t O, size_t ...N, typename ...Xs, typename ...Ys>
        void tuple_assign(std::tuple<Xs...> &lhs, std::index_sequence<N...>, Ys && ...ys)
        {
            std::initializer_list<bool> sink {(std::get<O+N>(lhs) = std::forward<Ys>(ys), true)...};
            (void)sink;
        }

        template <typename Fun, typename TupleT, size_t ...N>
        void tuple_foreach(Fun fun, TupleT &&tup, std::index_sequence<N...>)
        {
            std::initializer_list<bool> sink {(fun(std::get<N>(std::forward<TupleT>(tup))), true)...};
            (void)sink;
        }

        template <typename Fun, typename TupleT, size_t ...N>
        void tuple_foreach_index(Fun fun, TupleT &&tup, std::index_sequence<N...>)
        {
            std::initializer_list<bool> sink {(fun(std::integral_constant<size_t, N>{}, std::get<N>(std::forward<TupleT>(tup))), true)...};
            (void)sink;
        }

        template <typename Fun, typename TupleT, size_t ...N>
        auto tuple_map(Fun fun, TupleT &&tup, std::index_sequence<N...>)
        {
            return std::make_tuple(fun(std::get<N>(std::forward<TupleT>(tup)))...);
        }

        template <typename Fun, typename TupleT, size_t ...N>
        auto tuple_apply(Fun fun, TupleT &&tup, std::index_sequence<N...>)
        {
            return fun(std::get<N>(std::forward<TupleT>(tup))...);
        }
    }

    //
    // index_tuple: create indexes for a tuple
    //

    template <typename TupleT>
    using index_tuple = std::make_index_sequence<std::tuple_size<std::decay_t<TupleT>>::value>;


    //
    // tuple_assign: assign a pack to a tuple
    //

    template <typename ...Xs, typename ...Ys>
    void tuple_assign(std::tuple<Xs...> &lhs, Ys && ... ys)
    {
        return details::tuple_assign<0>(lhs, std::make_index_sequence<sizeof...(Ys)>(), std::forward<Ys>(ys)...);
    }

    template <size_t Offset, typename ...Xs, typename ...Ys>
    void tuple_assign_from(std::tuple<Xs...> &lhs, Ys && ... ys)
    {
        return details::tuple_assign<Offset>(lhs, std::make_index_sequence<sizeof...(Ys)>(), std::forward<Ys>(ys)...);
    }


    //
    // tuple_foreach: polymorphic actions over tuple
    //

    template <typename Fun, typename TupleT>
    void tuple_foreach(Fun fun, TupleT &&tup)
    {
        return details::tuple_foreach(fun, std::forward<TupleT>(tup), index_tuple<TupleT>{});
    }


    //
    // tuple_foreach_index: polymorphic actions over tuple with indexes
    //

    template <typename Fun, typename TupleT>
    void tuple_foreach_index(Fun fun, TupleT &&tup)
    {
        return details::tuple_foreach_index(fun, std::forward<TupleT>(tup), index_tuple<TupleT>{});
    }


    //
    // tuple_map: fmap over tuple
    //

    template <typename Fun, typename TupleT>
    auto tuple_map(Fun fun, TupleT &&tup)
    {
        return details::tuple_map(fun, std::forward<TupleT>(tup), index_tuple<TupleT>{});
    }


    //
    //  tuple_apply: expand a tuple into a pack and pass it to a callable
    //

    template <typename Fun, typename TupleT>
    auto tuple_apply(Fun fun, TupleT && tup)
    {
        return details::tuple_apply(fun, std::forward<TupleT>(tup), index_tuple<TupleT>{});
    }


    //
    // make_tuple: like std::make_tuple but possibly accepts a fewer number of arguments.
    // Missing arguments are default constructed.
    //

    template <typename ...Ts>
    inline std::tuple<Ts...>
    make_tuple()
    {
        return std::make_tuple(Ts{}...);
    }

    template <typename T, typename ...Ts, typename X, typename ...Xs>
    inline std::tuple<T, Ts...>
    make_tuple(X && x, Xs&& ... xs)
    {
        return std::tuple_cat(std::make_tuple(std::forward<X>(x)),
                              cat::make_tuple<Ts...>(std::forward<Xs>(xs)...));
    }

} // namespace cat
