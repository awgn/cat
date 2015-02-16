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

#include <cat/traits.hpp>
#include <cat/utility/tuple.hpp>

namespace cat
{
    //////////////////////////////////////////////////////////////////////////////////
    //
    // identity function
    //

    struct Identity
    {
        template <typename T>
        constexpr auto operator()(T &&x) const noexcept
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
        return std::function<typename callable_traits<F>::type>(std::forward<F>(f));
    }


    //////////////////////////////////////////////////////////////////////////////////
    //
    // _Callable with partial application support
    //

    template <typename C, typename ...Ts>
    struct _Callable
    {
        template <typename ...Xs>
        constexpr explicit _Callable(C fun, std::tuple<Xs...> args = std::tuple<Xs...>{})
        : fun_(std::move(fun))
        , args_(std::move(args))
        { }

        template <typename ...Xs>
        auto operator()(Xs && ... xs) const
        {
            constexpr size_t N = callable_traits<C>::arity - sizeof...(Ts);
            static_assert(N >= sizeof...(Xs), "Too many argument!");
            return eval_(std::integral_constant<size_t, N - sizeof...(Xs)>(), std::forward<Xs>(xs)...);
        }

        template <typename ...Xs>
        auto apply(Xs && ... xs) const
        {
            constexpr size_t N = callable_traits<C>::arity - sizeof...(Ts);
            static_assert(N >= sizeof...(Xs), "Too many argument!");
            return apply_(std::integral_constant<size_t, N - sizeof...(Xs)>(), std::forward<Xs>(xs)...);
        }

    private:
        template <typename ...Xs>
        auto eval_(std::integral_constant<size_t, 0>, Xs &&...xs) const
        {
            return tuple_apply(fun_, std::tuple_cat(args_, std::forward_as_tuple(std::forward<Xs>(xs)...)));
        }

        template <size_t I, typename ...Xs>
        auto eval_(std::integral_constant<size_t, I>, Xs &&...xs) const
        {
            return _Callable<C, Ts..., Xs...>(
                        fun_, std::tuple_cat(args_, std::forward_as_tuple(std::forward<Xs>(xs)...)));
        }

        template <size_t I, typename ...Xs>
        auto apply_(std::integral_constant<size_t, I>, Xs &&...xs) const
        {
            return _Callable<C, Ts..., Xs...>(
                        fun_, std::tuple_cat(args_, std::forward_as_tuple(std::forward<Xs>(xs)...)));
        }

        C fun_;
        std::tuple<Ts...> args_;
    };


    template<typename F>
    constexpr auto callable(F &&f)
    {
        return _Callable<F>(std::forward<F>(f));
    }


} // namespace cat

