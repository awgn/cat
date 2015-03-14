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

#include <functional>
#include <cat/functional.hpp>

namespace cat
{
    template <typename T, typename C,
              std::enable_if_t<is_callable<C>::value> * = nullptr>
    auto sec(T && value, C && call)
    {
        return cat::currying(std::forward<C>(call))(std::forward<T>(value));
    }

    template <typename C, typename T,
              std::enable_if_t<is_callable<C>::value> * = nullptr>
    auto sec(C && _call, T && _value)
    {
        return generic<placeholders::_a(placeholders::_a)>
                ([call = std::forward<C>(_call), rhs = std::forward<T>(_value)](auto && lhs)
                 {
                    return call(std::forward<decltype(lhs)>(lhs), std::move(rhs));
                });
    }

#if defined(__clang__) || (__GNUC__ >= 5)
    template <typename T> constexpr auto minus = std::minus<T>{};
    template <typename T> constexpr auto plus = std::plus<T>{};
    template <typename T> constexpr auto multiplies = std::multiplies<T>{};
    template <typename T> constexpr auto divides = std::divides<T>{};
    template <typename T> constexpr auto modulus = std::modulus<T>{};
    template <typename T> constexpr auto negate = std::negate<T>{};

    template <typename T> constexpr auto equal_to = std::equal_to<T>{};
    template <typename T> constexpr auto not_equal_to = std::not_equal_to<T>{};
    template <typename T> constexpr auto greater = std::greater<T>{};
    template <typename T> constexpr auto less = std::less<T>{};
    template <typename T> constexpr auto greater_equal = std::greater_equal<T>{};
    template <typename T> constexpr auto less_equal = std::less_equal<T>{};

    template <typename T> constexpr auto logical_and = std::logical_and<T>{};
    template <typename T> constexpr auto logical_or  = std::logical_or<T>{};
#endif

} // namespace cat
