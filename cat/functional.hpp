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
#include <cat/tuple.hpp>
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
    // Callable_ with partial application support
    //

    template <typename F, typename ...Ts>
    struct Callable_
    {
        using function_type =
            typename partial_function_type<
                typename function_type<F>::type, sizeof...(Ts)>::type;

        enum : size_t { arity_value = arity<F>::value - sizeof...(Ts) };

        template <typename ...Xs>
        constexpr explicit Callable_(F fun, std::tuple<Xs...> args = std::tuple<Xs...>{})
        : fun_(std::move(fun))
        , args_(std::move(args))
        { }

        template <typename ...Xs>
        auto operator()(Xs && ... xs) const
        {
            constexpr size_t N = arity<F>::value - sizeof...(Ts);
            static_assert(N >= sizeof...(Xs), "Too many argument!");
            return eval_(std::integral_constant<size_t, N - sizeof...(Xs)>(), std::forward<Xs>(xs)...);
        }

        template <typename ...Xs>
        auto apply(Xs && ... xs) const
        {
            constexpr size_t N = arity<F>::value - sizeof...(Ts);
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
            return Callable_<F, Ts..., Xs...>(
                        fun_, std::tuple_cat(args_, std::forward_as_tuple(std::forward<Xs>(xs)...)));
        }

        template <size_t I, typename ...Xs>
        auto apply_(std::integral_constant<size_t, I>, Xs &&...xs) const
        {
            return Callable_<F, Ts..., Xs...>(
                        fun_, std::tuple_cat(args_, std::forward_as_tuple(std::forward<Xs>(xs)...)));
        }

        F fun_;
        std::tuple<Ts...> args_;
    };

    template<typename F>
    constexpr auto callable(F f)
    {
        return Callable_<F>(std::move(f));
    }

    //////////////////////////////////////////////////////////////////////////////////
    //
    // Generic callable (for generic lambdas)
    //

    template <typename F, size_t Arity>
    struct Generic_
    {
        using function_type = unspec();
        enum : size_t { arity_value = Arity };

        constexpr Generic_(F f)
        : fun_(std::move(f))
        { }

        template <typename ...Ts>
        constexpr decltype(auto)
        operator()(Ts&& ... xs) const
        {
            return fun_(std::forward<Ts>(xs)...);
        }

        F fun_;
    };


    template <size_t A, typename F>
    constexpr auto generic(F f)
    {
        return Generic_<F, A>(std::move(f));
    };

    //////////////////////////////////////////////////////////////////////////////////
    //
    // Compose_: functional composition of callable types
    //

    template <typename F_, typename G_, typename = void>  struct Compose_;

    template <typename F_, typename G_>
    struct Compose_<F_, G_, typename std::enable_if<arity<G_>::value != 0>::type>
    {
        template <typename F, typename G>
        constexpr Compose_(F && f, G && g)
        : f_(std::forward<F>(f))
        , g_(std::forward<G>(g))
        { }

        template <typename T, typename ...Ts>
        constexpr decltype(auto)
        operator()(T &&x, Ts && ...xs) const
        {
            return f_( g_(std::forward<T>(x)), std::forward<Ts>(xs)...);
        }

        F_ f_;
        G_ g_;
    };

    template <typename F_, typename G_>
    struct Compose_<F_, G_, typename std::enable_if<arity<G_>::value == 0>::type>
    {
        template <typename F, typename G>
        constexpr Compose_(F && f, G && g)
        : f_(std::forward<F>(f))
        , g_(std::forward<G>(g))
        { }

        template <typename ...Ts>
        constexpr decltype(auto)
        operator()(Ts && ...xs) const
        {
            return f_( g_(), std::forward<Ts>(xs)...);
        }

        F_ f_;
        G_ g_;
    };

    template <typename F, typename G,
              typename std::enable_if<is_callable<F>::value && is_callable<G>::value>::type * = nullptr>
    constexpr auto compose(F f, G g)
    {
        return Compose_<F,G>{ std::move(f), std::move(g) };
    }

    template <typename F, typename G,
              typename std::enable_if<is_callable<F>::value && is_callable<G>::value>::type * = nullptr>
    constexpr auto operator^(F f, G g)
    {
        return compose(std::move(f), std::move(g));
    }

    //////////////////////////////////////////////////////////////////////////////////
    //
    // flip:
    //

    template <typename F>
    struct Flip_
    {
        constexpr Flip_(F fun)
        : fun_(std::move(fun))
        { }

        template <typename T1, typename T2, typename ...Ts>
        constexpr decltype(auto)
        operator()(T1 && x1, T2 && x2, Ts && ...xs) const
        {
            return fun_(std::forward<T2>(x2),
                        std::forward<T1>(x1),
                        std::forward<Ts>(xs)...);
        }

    private:
        F fun_;
    };

    template <typename F>
    constexpr auto flip(F fun_)
    {
        return Flip_<F>{ std::move(fun_) };
    };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // on (infix version):
    //

    template <typename F, typename G>
    struct On_
    {
        constexpr On_(F f, G g)
        : f_(std::move(f))
        , g_(std::move(g))
        { }

        template <typename T1, typename T2, typename ...Ts>
        constexpr decltype(auto)
        operator()(T1 && x1, T2 && x2, Ts && ...xs) const
        {
            return f_(g_(std::forward<T1>(x1)),
                      g_(std::forward<T2>(x2)), std::forward<Ts>(xs)...);
        }

    private:
        F f_;
        G g_;
    };

    struct on_
    {
        template <typename F, typename G>
        constexpr auto operator()(F f_, G g_) const
        {
            return On_<F,G>{ std::move(f_), std::move(g_) };
        };
    };


    constexpr auto on = infix(on_{});


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

} // namespace cat

