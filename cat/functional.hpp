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
#include <iterator>
#include <functional>

#include <cat/tuple.hpp>
#include <cat/type_traits.hpp>
#include <cat/infix.hpp>

namespace cat
{
    namespace placeholders
    {
        struct unspec { };

        struct A_  { } constexpr a_ = A_ { };
        struct B_  { } constexpr b_ = B_ { };
        struct C_  { } constexpr c_ = C_ { };
    };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // identity function
    //

    struct Identity
    {
        using function_type = placeholders::unspec(placeholders::unspec);

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
        void operator()(T const &elem) const
        {
            // TODO: use show typeclass...
            //
            std::cout << elem << std::endl;
        }

    } constexpr print = Print_{ };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // foldl, foldl1
    //

    struct Foldl_
    {
        template <typename F, typename A, template <typename ...> class C, typename ...Ts>
        auto operator()(F f, A acc, C<Ts...> const &xs) const
        {
            for(auto it = xs.cbegin(); it != xs.cend(); ++it)
                acc = f(std::move(acc), *it);

            return acc;
        }

    } constexpr foldl = Foldl_{ };

    struct Foldl1_
    {
        template <typename F, template <typename ...> class C, typename ...Ts>
        auto operator()(F f, C<Ts...> const &xs) const
        {
            if (xs.empty())
                throw std::runtime_error("foldl1: empty container");

            auto acc = xs.front();

            for(auto it = std::next(xs.cbegin()); it != xs.cend(); ++it)
                acc = f(std::move(acc), *it);

            return acc;
        }

    } constexpr foldl1 = Foldl1_{ };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // foldr, foldr1
    //

    struct Foldr_
    {
        template <typename F, typename A, template <typename ...> class C, typename ...Ts>
        auto operator()(F f, A acc, C<Ts...> const &xs) const
        {
            for(auto it = xs.crbegin(); it != xs.crend(); ++it)
                acc = f(*it, std::move(acc));

            return acc;
        }

    } constexpr foldr = Foldr_{ };


    struct Foldr1_
    {
        template <typename F, template <typename ...> class C, typename ...Ts>
        auto operator()(F f, C<Ts...> const &xs) const
        {
            if (xs.empty())
                throw std::runtime_error("foldr1: empty container");

            auto acc = xs.back();

            for(auto it = std::next(xs.crbegin()); it != xs.crend(); ++it)
                acc = f(*it, std::move(acc));

            return acc;
        }

    } constexpr foldr1 = Foldr1_{ };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // Callable_ with partial application support
    //

    template <typename T>
    struct callable_decay
    {
        using type = typename std::conditional<
                    std::is_lvalue_reference<T>::value && !std::is_const<std::remove_reference_t<T>>::value,
                        T,
                        std::decay_t<T>
                    >::type;
    };

    template <typename T>
    using callable_decay_t = typename callable_decay<T>::type;


    template <typename F, typename ...Ts>
    struct Callable_
    {
        using function_type =
            typename partial_function_type<
                typename function_type<F>::type, sizeof...(Ts)>::type;

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
            return Callable_<F, Ts..., callable_decay_t<Xs>...>(
                        fun_, std::tuple_cat(args_, std::forward_as_tuple(std::forward<Xs>(xs)...)));
        }

        template <size_t I, typename ...Xs>
        auto apply_(std::integral_constant<size_t, I>, Xs &&...xs) const
        {
            return Callable_<F, Ts..., callable_decay_t<Xs>...>(
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
    // Generic callable (for generic lambdas and polymorphic C++ callable objects)
    //

    template <typename Fun, typename F>
    struct Generic_
    {
        using function_type = Fun;

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

    template <typename Fun, typename F>
    constexpr auto generic(F f)
    {
        return callable(Generic_<Fun, F>(std::move(f)));
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
        template <typename Tp>
        constexpr T operator()(Tp const &) const
        {
            return value_;
        }

        T value_;
    };

    template <typename T>
    constexpr auto constant(T value)
    {
        return Constant_<T>{ std::move(value) };
    }


} // namespace cat
