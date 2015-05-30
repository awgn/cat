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
#include <utility>

namespace cat
{
    //
    // infix adaptors, inspired by FIT library
    //

    template <typename F>
    struct infix_adaptor
    {
        constexpr infix_adaptor(F o = F{})
        : oper(o)
        {}

        template <typename ...Ts>
        constexpr auto operator()(Ts && ...xs) const
        {
            return oper(std::forward<Ts>(xs)...);
        }

        F oper;
    };

    template <typename F>
    constexpr auto infix(F f)
    {
        return infix_adaptor<F>{std::move(f)};
    }


    template <typename L, typename F, std::size_t N> struct infix_left;

    template <typename L, typename F>
    struct infix_left<L, F, 5>
    {
        L ref;
        F oper;

        template <typename R>
        constexpr auto operator*(R && rhs)
        {
            return oper(std::forward<L>(ref), std::forward<R>(rhs));
        }

        template <typename R>
        constexpr auto operator/(R && rhs)
        {
            return oper(std::forward<L>(ref), std::forward<R>(rhs));
        }

        template <typename R>
        constexpr auto operator%(R && rhs)
        {
            return oper(std::forward<L>(ref), std::forward<R>(rhs));
        }
    };


    template <typename L, typename F>
    struct infix_left<L, F, 6>
    {
        L ref;
        F oper;

        template <typename R>
        constexpr auto operator+(R && rhs)
        {
            return oper(std::forward<L>(ref), std::forward<R>(rhs));
        }

        template <typename R>
        constexpr auto operator-(R && rhs)
        {
            return oper(std::forward<L>(ref), std::forward<R>(rhs));
        }
    };


    template <typename L, typename F>
    struct infix_left<L, F, 8>
    {
        L ref;
        F oper;

        template <typename R>
        constexpr auto operator>(R && rhs)
        {
            return oper(std::forward<L>(ref), std::forward<R>(rhs));
        }
    };


    template <typename L, typename F>
    struct infix_left<L, F, 10>
    {
        L ref;
        F oper;

        template <typename R>
        constexpr auto operator&(R && rhs)
        {
            return oper(std::forward<L>(ref), std::forward<R>(rhs));
        }
    };

    template <typename L, typename F>
    struct infix_left<L, F, 11>
    {
        L ref;
        F oper;

        template <typename R>
        constexpr auto operator^(R && rhs)
        {
            return oper(std::forward<L>(ref), std::forward<R>(rhs));
        }
    };

    template <typename L, typename F>
    struct infix_left<L, F, 12>
    {
        L ref;
        F oper;

        template <typename R>
        constexpr auto operator|(R && rhs)
        {
            return oper(std::forward<L>(ref), std::forward<R>(rhs));
        }
    };


    // infix 5:

    template <typename L, typename F>
    constexpr auto operator*(L && lhs, infix_adaptor<F> const & f)
    {
        return infix_left<std::add_rvalue_reference_t<L>, decltype(f.oper), 5>{ std::forward<L>(lhs), f.oper };
    }

    template <typename L, typename F>
    constexpr auto operator/(L && lhs, infix_adaptor<F> const & f)
    {
        return infix_left<std::add_rvalue_reference_t<L>, decltype(f.oper), 5>{ std::forward<L>(lhs), f.oper };
    }

    template <typename L, typename F>
    constexpr auto operator%(L && lhs, infix_adaptor<F> const & f)
    {
        return infix_left<std::add_rvalue_reference_t<L>, decltype(f.oper), 5>{ std::forward<L>(lhs), f.oper };
    }

    // infix 6:

    template <typename L, typename F>
    constexpr auto operator+(L && lhs, infix_adaptor<F> const & f)
    {
        return infix_left<std::add_rvalue_reference_t<L>, decltype(f.oper), 6>{ std::forward<L>(lhs), f.oper };
    }

    template <typename L, typename F>
    constexpr auto operator-(L && lhs, infix_adaptor<F> const & f)
    {
        return infix_left<std::add_rvalue_reference_t<L>, decltype(f.oper), 6>{ std::forward<L>(lhs), f.oper };
    }

    // infix 8:

    template <typename L, typename F>
    constexpr auto operator<(L && lhs, infix_adaptor<F> f)
    {
        return infix_left<std::add_rvalue_reference_t<L>, decltype(f.oper), 8>{ std::forward<L>(lhs), f.oper };
    }

    // infix 10:

    template <typename L, typename F>
    constexpr auto operator&(L && lhs, infix_adaptor<F> const & f)
    {
        return infix_left<std::add_rvalue_reference_t<L>, decltype(f.oper), 10>{ std::forward<L>(lhs), f.oper };
    }

    // infix 11:

    template <typename L, typename F>
    constexpr auto operator^(L && lhs, infix_adaptor<F> const & f)
    {
        return infix_left<std::add_rvalue_reference_t<L>, decltype(f.oper), 11>{ std::forward<L>(lhs), f.oper };
    }

    // infix 12:

    template <typename L, typename F>
    constexpr auto operator|(L && lhs, infix_adaptor<F> const & f)
    {
        return infix_left<std::add_rvalue_reference_t<L>, decltype(f.oper), 12>{ std::forward<L>(lhs), f.oper };
    }

} // namespace cat
