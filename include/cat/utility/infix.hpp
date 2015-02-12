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

#include <type_traits>
#include <utility>

namespace cat
{
    //
    // simple infix adapters, inspired by FIT library
    //

    template <typename F>
    struct infix_adaptor
    {
        infix_adaptor(F o = F{})
        : oper(o)
        {}

        F oper;
    };

    template <typename F>
    inline auto infix(F f)
    {
        return infix_adaptor<F>{f};
    }

    template <typename L, typename F>
    struct infix_left
    {
        L left;
        F oper;

        template <typename R>
        inline auto operator*(R && rhs)
        {
            return oper(std::move(left), std::forward<R>(rhs));
        }

        template <typename R>
        inline auto operator/(R && rhs)
        {
            return oper(std::move(left), std::forward<R>(rhs));
        }

        template <typename R>
        inline auto operator%(R && rhs)
        {
            return oper(std::move(left), std::forward<R>(rhs));
        }

        template <typename R>
        inline auto operator+(R && rhs)
        {
            return oper(std::move(left), std::forward<R>(rhs));
        }

        template <typename R>
        inline auto operator-(R && rhs)
        {
            return oper(std::move(left), std::forward<R>(rhs));
        }

        template <typename R>
        inline auto operator>(R && rhs)
        {
            return oper(std::move(left), std::forward<R>(rhs));
        }

        template <typename R>
        inline auto operator&(R && rhs)
        {
            return oper(std::move(left), std::forward<R>(rhs));
        }

        template <typename R>
        inline auto operator^(R && rhs)
        {
            return oper(std::move(left), std::forward<R>(rhs));
        }

        template <typename R>
        inline auto operator|(R && rhs)
        {
            return oper(std::move(left), std::forward<R>(rhs));
        }
    };

    // infix 5:

    template <typename L, typename F>
    inline auto operator*(L && lhs, infix_adaptor<F> f)
    {
        return infix_left<std::decay_t<L>, decltype(f.oper)>{ std::forward<L>(lhs), f.oper };
    }

    template <typename L, typename F>
    inline auto operator/(L && lhs, infix_adaptor<F> f)
    {
        return infix_left<std::decay_t<L>, decltype(f.oper)>{ std::forward<L>(lhs), f.oper };
    }

    template <typename L, typename F>
    inline auto operator%(L && lhs, infix_adaptor<F> f)
    {
        return infix_left<std::decay_t<L>, decltype(f.oper)>{ std::forward<L>(lhs), f.oper };
    }

    // infix 6:

    template <typename L, typename F>
    inline auto operator+(L && lhs, infix_adaptor<F> f)
    {
        return infix_left<std::decay_t<L>, decltype(f.oper)>{ std::forward<L>(lhs), f.oper };
    }

    template <typename L, typename F>
    inline auto operator-(L && lhs, infix_adaptor<F> f)
    {
        return infix_left<std::decay_t<L>, decltype(f.oper)>{ std::forward<L>(lhs), f.oper };
    }

    // infix 8:

    template <typename L, typename F>
    inline auto operator<(L && lhs, infix_adaptor<F> f)
    {
        return infix_left<std::decay_t<L>, decltype(f.oper)>{ std::forward<L>(lhs), f.oper };
    }

    // infix 10:

    template <typename L, typename F>
    inline auto operator&(L && lhs, infix_adaptor<F> f)
    {
        return infix_left<std::decay_t<L>, decltype(f.oper)>{ std::forward<L>(lhs), f.oper };
    }

    // infix 11:

    template <typename L, typename F>
    inline auto operator^(L && lhs, infix_adaptor<F> f)
    {
        return infix_left<std::decay_t<L>, decltype(f.oper)>{ std::forward<L>(lhs), f.oper };
    }

    // infix 12:

    template <typename L, typename F>
    inline auto operator|(L && lhs, infix_adaptor<F> f)
    {
        return infix_left<std::decay_t<L>, decltype(f.oper)>{ std::forward<L>(lhs), f.oper };
    }

} // namespace cat
