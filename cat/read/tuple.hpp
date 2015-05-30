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

#include <cat/read/read.hpp>
#include <cat/monad/optional.hpp>
#include <cat/tuple.hpp>

#include <type_traits>

namespace cat
{
    //
    // Instances...
    //

    template <typename T1, typename T2>
    struct ReadInstance<std::pair<T1,T2>> final : Read<std::pair<T1, T2>>
    {
        optional<std::pair<std::pair<T1,T2>,string_view>>
        reads(string_view v) override
        {
            //
            // C++ way...
            //
            // auto v1 = consume_char('(', v);
            // if (!v1)
            //     return nullopt;
            // auto t1 = cat::reads<T1>(v1.value());
            // if (!t1)
            //     return nullopt;
            // auto t2 = cat::reads<T2>(t1.value().second);
            // if (!t2)
            //     return nullopt;
            // auto c2 = consume_char(')', t2.value().second);
            // if (!c2)
            //     return nullopt;
            // return make_optional(std::make_pair(std::make_pair(t1.value().first, t2.value().first), c2.value()));

            //
            // The functional way... desugared monads!
            //

            // return consume_char('(', v) >>= [](string_view s1) {
            //     return cat::reads<T1>(s1) >>= [&] (auto const &t1) {
            //         return cat::reads<T2>(t1.second) >>= [&] (auto const &t2) {
            //             return consume_char(')', t2.second) >>= [&](string_view left) {
            //                 return mreturn.in<optional>(std::make_pair(std::make_pair(t1.first, t2.first), left) );
            //             };
            //         };
            //     };
            // };

            //
            // sugared monad...
            //

            return DO( s1,   (consume_char('(', v)),
                   DO( t1,   (cat::reads<T1>(s1)),
                   DO( t2,   (cat::reads<T2>(t1.second)),
                   DO(left,  (consume_char(')', t2.second)),
                      (
                            mreturn.in<optional>(std::make_pair(std::make_pair(std::move(t1.first), std::move(t2.first)), std::move(left)));
                      )))));
        }
    };


    template <typename ...Ts>
    struct ReadInstance<std::tuple<Ts...>> final : Read<std::tuple<Ts...>>
    {
        optional<std::pair<std::tuple<Ts...>,string_view>>
        reads(string_view s) override
        {
            if (auto s_ = consume_char('(', s))
            {
                s = std::move(s_.value());

                std::tuple<Ts...> ret;

                size_t cnt = 0;

                tuple_foreach([&](auto &elem) {
                    if (auto val = cat::reads<std::decay_t<decltype(elem)>>(s))
                    {
                        elem = std::move(val.value().first);
                        s = val.value().second;
                        cnt++;
                    }
                }, ret);

                if (auto left = consume_char(')', s))
                {
                    if (cnt == sizeof...(Ts))
                        return mreturn.in<optional>(std::make_pair(std::move(ret), std::move(left.value())));
                }
            }

            return nullopt;
        }
    };

}
