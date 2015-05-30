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

namespace cat
{
    namespace type
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

    } // namespace type

} // namespace cat

