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
#include <cat/tuple.hpp>
#include <cat/placeholders.hpp>
#include <cat/type_traits.hpp>
#include <cat/infix.hpp>
#include <cat/utility.hpp>
#include <cat/bits/missing.hpp>

#include <utility>
#include <iterator>
#include <functional>

namespace cat
{

    //////////////////////////////////////////////////////////////////////////////////
    //
    // identity function
    //

    struct Identity
    {
        using function_type = placeholders::_a(placeholders::_a);

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
        using function_type = placeholders::_a(std::pair<placeholders::_a, placeholders::_b>);

        template <typename P>
        constexpr decltype(auto)
        operator()(P && p) const noexcept
        {
            return std::get<0>(std::forward<P>(p));
        }

    } constexpr first = First_{};


    struct Second_
    {
        using function_type = placeholders::_b(std::pair<placeholders::_a, placeholders::_b>);

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
        using function_type = placeholders::unspec(std::tuple<placeholders::unspec>);

        template <typename Tuple>
        constexpr decltype(auto)
        operator()(Tuple && t) const noexcept
        {
            return std::get<N>(std::forward<Tuple>(t));
        }
    };


#if defined(__clang__)

    template <size_t N>
    constexpr auto elem = Elem_<N>{};

#endif

    //////////////////////////////////////////////////////////////////////////////////
    //
    // Callable_ with partial application support
    //

    namespace details
    {
        //
        // Argument decay on the basis of the target signature.
        //

        template <typename F, size_t Idx, typename T>
        struct currying_decay
        {
            using Tp = arg_type_at_t<Idx, F>;

            using type = typename std::conditional<
                            std::is_lvalue_reference<Tp>::value,
                            T, std::decay_t<T> >::type;
        };

        template <typename F, size_t Idx, typename T>
        using currying_decay_t = typename currying_decay<F, Idx, T>::type;
    }


    template <typename F, typename ...Ts>
    struct Callable_
    {
        using function_type =
            typename meta::partial_function_type<
                typename function_type<F>::type, sizeof...(Ts)>::type;

        template <typename ...Xs>
        constexpr explicit Callable_(F fun, std::tuple<Xs...> args = std::tuple<Xs...>{})
        : fun_(std::move(fun))
        , args_(std::move(args))
        { }

        template <typename ...Xs>
        auto operator()(Xs && ... xs) const
        {
            constexpr size_t N = function_arity<F>::value - sizeof...(Ts);
            static_assert(N >= sizeof...(Xs), "Too many argument!");
            return eval_(std::integral_constant<size_t, N - sizeof...(Xs)>(), std::make_index_sequence<sizeof...(Xs)>(), std::forward<Xs>(xs)...);
        }

        template <typename ...Xs>
        auto apply(Xs && ... xs) const
        {
            constexpr size_t N = function_arity<F>::value - sizeof...(Ts);
            static_assert(N >= sizeof...(Xs), "Too many argument!");
            return apply_(std::integral_constant<size_t, N - sizeof...(Xs)>(), std::make_index_sequence<sizeof...(Xs)>(), std::forward<Xs>(xs)...);
        }

    private:
        template <size_t ...N, typename ...Xs>
        auto eval_(std::integral_constant<size_t, 0>, std::index_sequence<N...>, Xs &&...xs) const
        {
            return tuple_apply(fun_, std::tuple_cat(args_, std::forward_as_tuple(std::forward<Xs>(xs)...)));
        }

        template <size_t I, size_t ...N, typename ...Xs>
        auto eval_(std::integral_constant<size_t, I>, std::index_sequence<N...>, Xs &&...xs) const
        {
            return Callable_<F, Ts..., details::currying_decay_t<function_type, N, Xs>...>(
                        fun_, std::tuple_cat(args_, std::forward_as_tuple(std::forward<Xs>(xs)...)));
        }

        template <size_t I, size_t ...N, typename ...Xs>
        auto apply_(std::integral_constant<size_t, I>, std::index_sequence<N...>, Xs &&...xs) const
        {
            return Callable_<F, Ts..., details::currying_decay_t<function_type, N, Xs>...>(
                        fun_, std::tuple_cat(args_, std::forward_as_tuple(std::forward<Xs>(xs)...)));
        }

        F fun_;
        std::tuple<Ts...> args_;
    };


    template<typename F>
    constexpr auto curry(F && f)
    {
        return Callable_<std::decay_t<F>>(std::forward<F>(f));
    }
    template<typename F, typename ...Ts>
    constexpr auto curry(F && f, Ts&& ...xs)
    {
        return curry(std::forward<F>(f)).apply(std::forward<Ts>(xs)...);
    }


    //////////////////////////////////////////////////////////////////////////////////
    //
    // Generic callable (for generic lambdas and polymorphic C++ callable objects)
    //

    template <typename Fun, typename F>
    struct Callable_as_
    {
        using function_type = Fun;

        constexpr Callable_as_()
        : fun_{}
        { }

        constexpr Callable_as_(F f)
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

    template <typename Fun, typename F>
    constexpr auto curry_as(F && f)
    {
        return curry(Callable_as_<Fun, std::decay_t<F>>(std::forward<F>(f)));
    };
    template <typename Fun, typename F, typename ...Ts>
    constexpr auto curry_as(F && f, Ts && ... xs)
    {
        return curry_as<Fun>(std::forward<F>(f)).apply(std::forward<Ts>(xs)...);
    };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // Compose_: functional composition of callable types
    //

    template <typename F, typename G, typename = void>  struct Compose_;

    template <typename F, typename G>
    struct Compose_<F, G, std::enable_if_t<function_arity<G>::value != 0>>
    {
       using function_type = meta::compose_function_type_t<
                                function_type_t<F>,
                                function_type_t<G>>;

        constexpr Compose_(F f, G g)
        : f_(std::move(f))
        , g_(std::move(g))
        { }

        template <typename T, typename ...Ts>
        constexpr decltype(auto)
        operator()(T &&x, Ts && ...xs) const
        {
            return f_( g_(std::forward<T>(x)), std::forward<Ts>(xs)...);
        }

        F f_;
        G g_;
    };

    template <typename F, typename G>
    struct Compose_<F, G, std::enable_if_t<function_arity<G>::value == 0>>
    {
        using function_type = meta::compose_function_type_t<
                                function_type_t<F>,
                                function_type_t<G>>;

        constexpr Compose_(F f, G g)
        : f_(std::move(f))
        , g_(std::move(g))
        { }

        template <typename ...Ts>
        constexpr decltype(auto)
        operator()(Ts && ...xs) const
        {
            return f_( g_(), std::forward<Ts>(xs)...);
        }

        F f_;
        G g_;
    };

    template <typename F, typename G>
    constexpr auto compose(F && f, G && g)
    {
        static_assert(is_callable<F>::value && is_callable<G>::value,
                      "compose: F and G must be callable types");

        return Compose_<std::decay_t<F>,std::decay_t<G>>{ std::forward<F>(f), std::forward<G>(g) };
    }

    template <typename F, typename G,
              std::enable_if_t<is_callable<F>::value && is_callable<G>::value> * = nullptr>
    constexpr auto operator^(F && f, G && g)
    {
        return compose(std::forward<F>(f), std::forward<G>(g));
    }

    //////////////////////////////////////////////////////////////////////////////////
    //
    // flip:
    //

    template <typename F>
    struct Flip_
    {
        using function_type = meta::flip_function_type_t<function_type_t<F>>;

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
    constexpr auto flip(F && fun_)
    {
        return Flip_<std::decay_t<F>>{ std::forward<F>(fun_) };
    };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // on (infix version):
    //

    template <typename F, typename G>
    struct On_
    {
        using function_type = meta::on_function_type_t<function_type_t<F>,
                                                 function_type_t<G>>;

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
        constexpr auto
        operator()(F f_, G g_) const
        {
            return On_<F,G>{ std::move(f_), std::move(g_) };
        };
    };

    constexpr auto on = infix(on_{});

    //////////////////////////////////////////////////////////////////////////////////
    //
    // constant
    //

    template <typename T>
    struct Constant_
    {
        using function_type = T(placeholders::unspec);

        template <typename Tp>
        constexpr T operator()(Tp const &) const
        {
            return value_;
        }

        T value_;
    };

    template <typename T>
    constexpr auto constant(T && value)
    {
        return Constant_<std::decay_t<T>>{ std::forward<T>(value) };
    }

} // namespace cat
