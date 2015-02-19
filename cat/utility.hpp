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
#include <functional>

#include <cat/type_traits.hpp>
#include <cat/infix.hpp>

namespace cat
{
    //////////////////////////////////////////////////////////////////////////////////
    //
    // convertible type useful in unevaluated operands
    //

    struct unspec
    {
        template <typename T> operator T();
    };

    //////////////////////////////////////////////////////////////////////////////////
    //
    // identity function
    //

    struct Identity
    {
        using function_type = unspec(unspec);
        enum : size_t { arity_value = 1 };

        template <typename T>
        constexpr auto
        operator()(T &&x) const noexcept
        {
            return std::forward<T>(x);
        }
    };

    constexpr Identity identity = {};


    //////////////////////////////////////////////////////////////////////////////////
    //
    // make_function
    //

    template<typename F>
    auto constexpr make_function(F &&f)
    {
        return std::function<typename function_type<F>::type>(std::forward<F>(f));
    }

    //////////////////////////////////////////////////////////////////////////////////
    //
    // first, second, elem<> on pair/tuple...:
    //

    struct First_
    {
        template <typename P>
        constexpr decltype(auto)
        operator()(P && p) const noexcept
        {
            return std::get<0>(std::forward<P>(p));
        }

    } constexpr first = First_{};

    struct Second_
    {
        template <typename P>
        constexpr decltype(auto)
        operator()(P &&p) const noexcept
        {
            return std::get<1>(std::forward<P>(p));
        }

    } constexpr second = Second_{};


    template <size_t N>
    struct Elem_
    {
        template <typename Tuple>
        constexpr decltype(auto)
        operator()(Tuple && t) const noexcept
        {
            return std::get<N>(std::forward<Tuple>(t));
        }
    };

#ifdef __clang

    template <size_t N>
    constexpr auto elem = Elem_<N>{};

#endif


    //////////////////////////////////////////////////////////////////////////////////
    //
    // print:
    //

    struct Print_
    {
        template <typename T>
        void operator()(T const &elem)
        {
            // TODO: use show typeclass...
            //
            std::cout << elem << std::endl;
        }

    } constexpr print = Print_{ };

} // namespace cat
