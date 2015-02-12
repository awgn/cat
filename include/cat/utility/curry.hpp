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

#include <utility>
#include <type_traits>

namespace cat {

    template <typename C, typename T>
    struct _curry_type
    {
        template <typename Ti>
        _curry_type(C call, Ti && a)
        : call_(std::move(call))
        , a_(std::forward<Ti>(a))
        {}

        _curry_type(_curry_type const &) = default;
        _curry_type(_curry_type &&) = default;
        ~_curry_type() = default;

        template <typename ... Ti>
        auto operator()(Ti && ... as)
        {
            return call_(std::forward<T>(a_), std::forward<Ti>(as)...);
        }

    private:

        C call_;
        T a_;
    };

    //
    // Simple curry
    //

    template <typename C, typename T>
    auto curry(C call, T && a)
    {
        return _curry_type<C, T>(std::move(call), std::forward<T>(a));
    }

    template <typename C, typename T, typename ... Ts>
    auto curry(C call, T && a, Ts && ... as)
    {
        return curry(curry(std::move(call), std::forward<T>(a)), std::forward<Ts>(as)...);
    }

    //
    // closure curry
    //

    template <typename T>
    using _closure_type = typename std::add_const<typename std::decay<T>::type>::type;


    template <typename C, typename T>
    auto closure(C call, T && a)
    {
        return _curry_type<C, _closure_type<T>>(call, std::forward<T>(a));
    }

    template <typename C, typename T, typename ... Ts>
    auto closure(C call, T && a, Ts && ... as)
    {
        return closure(closure(call, std::forward<T>(a)), std::forward<Ts>(as)...);
    }

} // namespace cat

