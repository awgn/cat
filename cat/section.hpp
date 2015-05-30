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
#include <cat/functional.hpp>
#include <cat/placeholders.hpp>

#include <functional>

namespace cat
{
    template <typename T, typename C,
              std::enable_if_t<is_callable<C>::value> * = nullptr>
    auto sec(T && value, C && call)
    {
        return cat::curry(std::forward<C>(call))(std::forward<T>(value));
    }

    template <typename C, typename T,
              std::enable_if_t<is_callable<C>::value> * = nullptr>
    auto sec(C && _call, T && _value)
    {
        return curry_as<placeholders::_a(placeholders::_a)>
                ([call = std::forward<C>(_call), rhs = std::forward<T>(_value)](auto && lhs)
                 {
                    return call(std::forward<decltype(lhs)>(lhs), std::move(rhs));
                });
    }

    template <typename T>
    auto operator+(placeholders::__, T const &val) { return sec(curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::plus<>{}), val); }
    template <typename T>
    auto operator+(T const &val, placeholders::__) { return sec(val, curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::plus<>{})); }
    inline auto operator+(placeholders::__, placeholders::__) { return curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::plus<>{}); }

    template <typename T>
    auto operator-(placeholders::__, T const &val) { return sec(curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::minus<>{}), val); }
    template <typename T>
    auto operator-(T const &val, placeholders::__) { return sec(val, curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::minus<>{})); }
    inline auto operator-(placeholders::__, placeholders::__) { return curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::minus<>{}); }

    template <typename T>
    auto operator*(placeholders::__, T const &val) { return sec(curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::multiplies<>{}), val); }
    template <typename T>
    auto operator*(T const &val, placeholders::__) { return sec(val, curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::multiplies<>{})); }
    inline auto operator*(placeholders::__, placeholders::__) { return curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::multiplies<>{}); }


    template <typename T>
    auto operator/(placeholders::__, T const &val) { return sec(curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::divides<>{}), val); }
    template <typename T>
    auto operator/(T const &val, placeholders::__) { return sec(val, curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::divides<>{})); }
    inline auto operator/(placeholders::__, placeholders::__) { return curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::divides<>{}); }

    template <typename T>
    auto operator%(placeholders::__, T const &val) { return sec(curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::modulus<>{}), val); }
    template <typename T>
    auto operator%(T const &val, placeholders::__) { return sec(val, curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::modulus<>{})); }
    inline auto operator%(placeholders::__, placeholders::__) { return curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::modulus<>{}); }

    template <typename T>
    auto operator==(placeholders::__, T const &val) { return sec(curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::equal_to<>{}), val); }
    template <typename T>
    auto operator==(T const &val, placeholders::__) { return sec(val, curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::equal_to<>{})); }
    inline auto operator==(placeholders::__, placeholders::__) { return curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::equal_to<>{}); }

    template <typename T>
    auto operator!=(placeholders::__, T const &val) { return sec(curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::not_equal_to<>{}), val); }
    template <typename T>
    auto operator!=(T const &val, placeholders::__) { return sec(val, curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::not_equal_to<>{})); }
    inline auto operator!=(placeholders::__, placeholders::__) { return curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::not_equal_to<>{}); }

    template <typename T>
    auto operator>(placeholders::__, T const &val) { return sec(curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::greater<>{}), val); }
    template <typename T>
    auto operator>(T const &val, placeholders::__) { return sec(val, curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::greater<>{})); }
    inline auto operator>(placeholders::__, placeholders::__) { return curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::greater<>{}); }

    template <typename T>
    auto operator>=(placeholders::__, T const &val) { return sec(curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::greater_equal<>{}), val); }
    template <typename T>
    auto operator>=(T const &val, placeholders::__) { return sec(val, curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::greater_equal<>{})); }
    inline auto operator>=(placeholders::__, placeholders::__) { return curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::greater_equal<>{}); }

    template <typename T>
    auto operator<(placeholders::__, T const &val) { return sec(curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::less<>{}), val); }
    template <typename T>
    auto operator<(T const &val, placeholders::__) { return sec(val, curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::less<>{})); }
    inline auto operator<(placeholders::__, placeholders::__) { return curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::less<>{}); }

    template <typename T>
    auto operator<=(placeholders::__, T const &val) { return sec(curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::less_equal<>{}), val); }
    template <typename T>
    auto operator<=(T const &val, placeholders::__) { return sec(val, curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::less_equal<>{})); }
    inline auto operator<=(placeholders::__, placeholders::__) { return curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::less_equal<>{}); }

    template <typename T>
    auto operator&&(placeholders::__, T const &val) { return sec(curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::logical_and<>{}), val); }
    template <typename T>
    auto operator&&(T const &val, placeholders::__) { return sec(val, curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::logical_and<>{})); }
    inline auto operator&&(placeholders::__, placeholders::__) { return curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::logical_and<>{}); }

    template <typename T>
    auto operator||(placeholders::__, T const &val) { return sec(curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::logical_or<>{}), val); }
    template <typename T>
    auto operator||(T const &val, placeholders::__) { return sec(val, curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::logical_or<>{})); }
    inline auto operator||(placeholders::__, placeholders::__) { return curry_as<placeholders::_a(placeholders::_a, placeholders::_a)>(std::logical_or<>{}); }


#if defined(__clang__)
    template <typename T = void> constexpr auto minus         = std::conditional_t<std::is_same<T,void>::value, Callable_as_<placeholders::_a(placeholders::_a, placeholders::_a), std::minus<T>>, std::minus<T>> {};
    template <typename T = void> constexpr auto plus          = std::conditional_t<std::is_same<T,void>::value, Callable_as_<placeholders::_a(placeholders::_a, placeholders::_a), std::plus<T>>, std::plus<T>> {};
    template <typename T = void> constexpr auto multiplies    = std::conditional_t<std::is_same<T,void>::value, Callable_as_<placeholders::_a(placeholders::_a, placeholders::_a), std::multiplies<T>>, std::multiplies<T>> {};
    template <typename T = void> constexpr auto divides       = std::conditional_t<std::is_same<T,void>::value, Callable_as_<placeholders::_a(placeholders::_a, placeholders::_a), std::divides<T>>, std::divides<T>> {};
    template <typename T = void> constexpr auto modulus       = std::conditional_t<std::is_same<T,void>::value, Callable_as_<placeholders::_a(placeholders::_a, placeholders::_a), std::modulus<T>>, std::modulus<T>> {};
    template <typename T = void> constexpr auto negate        = std::conditional_t<std::is_same<T,void>::value, Callable_as_<placeholders::_a(placeholders::_a, placeholders::_a), std::negate<T>>, std::negate<T>> {};
    template <typename T = void> constexpr auto equal_to      = std::conditional_t<std::is_same<T,void>::value, Callable_as_<placeholders::_a(placeholders::_a, placeholders::_a), std::equal_to<T>>, std::equal_to<T>> {};
    template <typename T = void> constexpr auto not_equal_to  = std::conditional_t<std::is_same<T,void>::value, Callable_as_<placeholders::_a(placeholders::_a, placeholders::_a), std::not_equal_to<T>>, std::not_equal_to<T>> {};
    template <typename T = void> constexpr auto greater       = std::conditional_t<std::is_same<T,void>::value, Callable_as_<placeholders::_a(placeholders::_a, placeholders::_a), std::greater<T>>, std::greater<T>> {};
    template <typename T = void> constexpr auto less          = std::conditional_t<std::is_same<T,void>::value, Callable_as_<placeholders::_a(placeholders::_a, placeholders::_a), std::less<T>>, std::less<T>> {};
    template <typename T = void> constexpr auto greater_equal = std::conditional_t<std::is_same<T,void>::value, Callable_as_<placeholders::_a(placeholders::_a, placeholders::_a), std::greater_equal<T>>, std::greater_equal<T>> {};
    template <typename T = void> constexpr auto less_equal    = std::conditional_t<std::is_same<T,void>::value, Callable_as_<placeholders::_a(placeholders::_a, placeholders::_a), std::less_equal<T>>, std::less_equal<T>> {};
    template <typename T = void> constexpr auto logical_and   = std::conditional_t<std::is_same<T,void>::value, Callable_as_<placeholders::_a(placeholders::_a, placeholders::_a), std::logical_and<T>>, std::logical_and<T>> {};
    template <typename T = void> constexpr auto logical_or    = std::conditional_t<std::is_same<T,void>::value, Callable_as_<placeholders::_a(placeholders::_a, placeholders::_a), std::logical_or<T>>, std::logical_or<T>> {};
#endif

} // namespace cat
