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

#include <type_traits>

namespace cat
{
    template <typename T>
    decltype(auto)
    const_forward(typename std::remove_reference<T>::type &value)
    {
        return static_cast<const std::remove_reference_t<T> &>(value);
    }

    template <typename T>
    decltype(auto)
    const_forward(typename std::remove_reference<T>::type &&value)
    {
        return static_cast<T &&>(value);
    }


    template<typename T, typename V>
    decltype(auto)
    forward_as(V && value)
    {
        return static_cast<
            std::conditional_t<
                std::is_lvalue_reference<T>::value,
                    std::remove_reference_t<V> &,
                    std::remove_reference_t<V> &&>>(value);
    }

} // namespace cat
