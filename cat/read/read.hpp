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

#include <cat/type_traits.hpp>
#include <cat/bits/type.hpp>
#include <cat/string_view.hpp>
#include <cat/optional.hpp>

namespace cat
{
    //
    // instance
    //

    template <typename T> struct ReadInstance;

    //
    // trait for concepts
    //

    template <typename T>
    struct is_readable : has_specialization<ReadInstance, T>
    { };

    //
    // type class Read
    //

    template <typename T>
    struct Read
    {
        virtual optional<std::pair<T, string_view>> reads(string_view) = 0;
    };

    //
    // free functions
    //

    template <typename T>
    T read(string_view str)
    {
        static_assert(is_readable<T>::value, "T is not readable!");
        auto r = ReadInstance<T>{}.reads(str);
        if (!r)
            throw std::runtime_error(type_name<T>() + ": read error!");
        return r.value().first;
    }

    template <typename T>
    optional<std::pair<T, string_view>>
    reads(string_view str)
    {
        static_assert(is_readable<T>::value, "T is not readable!");
        return ReadInstance<T>{}.reads(str);
    }

    namespace details
    {
        template <size_t N, typename ...Ts>
        optional<std::pair<size_t, string_view>>
        read_any(std::tuple<Ts...> &, string_view, std::false_type)
        {
            return nullopt;
        }

        template <size_t N, typename ...Ts>
        optional<std::pair<size_t, string_view>>
        read_any(std::tuple<Ts...> &t, string_view str, std::true_type)
        {
            auto n = cat::reads<cat::type_at_t<N, Ts...>>(str);
            if (n)
            {
                std::get<N>(t) = std::move(n->first);
                return make_optional(std::make_pair(N, n->second));
            }

            return read_any<N+1>(t, str, std::integral_constant<bool, (N+1) < sizeof...(Ts)>{});
        }
    }

    template <typename ...Ts>
    optional<std::pair<size_t, string_view>>
    read_any(std::tuple<Ts...> &t, string_view s)
    {
        return details::read_any<0>(t, s, std::integral_constant<bool, true>{});
    }

}
