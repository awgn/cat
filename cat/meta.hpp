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

#include <experimental/string_view>
#include <vector>

namespace cat {  namespace meta {

    //////////////////////////////////////////////////////////////////////////////////
    //
    // the identity meta-function
    //

    template <typename T>
    struct identity
    {
        using type = T;
    };

    //////////////////////////////////////////////////////////////////////////////////
    //
    // result_of: std::result_of meta-function adaptor
    //

    template <typename Fun, typename ...Ts>
    struct result_of
    {
        using type = typename std::result_of<Fun(Ts...)>::type;
    };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // partial_function_type
    //

    template <typename F, size_t N, typename = void > struct partial_function_type;

    template <typename R, typename T, typename ...Ts, size_t N>
    struct partial_function_type<R(T, Ts...), N, std::enable_if_t<(N != 0)>>
        : partial_function_type<R(Ts...), N-1> { };

    template <typename R, typename ...Ts, size_t N>
    struct partial_function_type<R(Ts...), N, std::enable_if_t<(N == 0)>>
    {
        using type = R(Ts...);
    };

    template <typename F, size_t N>
    using partial_function_type_t = typename partial_function_type<F, N>::type;


    //////////////////////////////////////////////////////////////////////////////////
    //
    // compose_function_type
    //

    template <typename F, typename G> struct compose_function_type;

    template <typename Fr, typename Fx, typename ...Fxs, typename Gr, typename Gx, typename ...Gxs>
    struct compose_function_type<Fr(Fx, Fxs...), Gr(Gx, Gxs...)>
    {
        using type = Fr(Gx, Fxs...);
    };
    template <typename Fr, typename Fx, typename ...Fxs, typename Gr>
    struct compose_function_type<Fr(Fx, Fxs...), Gr()>
    {
        using type = Fr(Fxs...);
    };

    template <typename F, typename G>
    using compose_function_type_t = typename compose_function_type<F, G>::type;


    //////////////////////////////////////////////////////////////////////////////////
    //
    // flip_function_type
    //

    template <typename F> struct flip_function_type;

    template <typename R, typename T0, typename T1, typename ...Ts>
    struct flip_function_type<R(T0, T1, Ts...)>
    {
        using type = R(T1, T0, Ts...);
    };

    template <typename F>
    using flip_function_type_t = typename flip_function_type<F>::type;


    //////////////////////////////////////////////////////////////////////////////////
    //
    // on_function_type
    //

    template <typename F, typename G> struct on_function_type;

    template <typename R, typename T1, typename T2, typename ...Ts, typename T, typename Gx>
    struct on_function_type<R(T1, T2, Ts...), T(Gx)>
    {
        using type = R(Gx, Gx, Ts...);
    };

    template <typename F, typename G>
    using on_function_type_t = typename on_function_type<F,G>::type;


    ///////////////////////////////////////////////////////////////////////////////////
    //
    // apply a meta-predicate on the outer type (type constructor, e.g. functor, monad)
    // of the given type
    //

    template < template <template <typename ...> class> class Trait, typename T>
    struct on_outer_type;

    template < template <template <typename ...> class> class Trait, template <typename ...> class Outer, typename ...Ts>
    struct on_outer_type<Trait, Outer<Ts...>> : Trait<Outer> { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // apply argument(s) to a meta function
    //

    template <template <typename ...> class F, typename ...Ts>
    struct apply
    {
        template <typename ...Rs>
        using type = F<Ts..., Rs...>;
    };


    ///////////////////////////////////////////////////////////////////////////////
    //
    // apply a meta-function(s) over the inner types
    //

    template <typename Functor, template <typename...> class ...Fs>
    struct fmap;

    template <template <typename ...> class Functor, typename T, typename ...Ts, template <typename...> class F>
    struct fmap<Functor<T,Ts...>, F>
    {
        using type = Functor<typename F<T>::type, Ts...>;
    };

    template <template <typename ...> class Functor, typename T0, typename T, typename ...Ts, template <typename...> class F0, template <typename...> class F>
    struct fmap<Functor<T0, T, Ts...>, F0, F>
    {
        using type = Functor<typename F0<T0>::type, typename F<T>::type, Ts...>;
    };
    template <template <typename ...> class Functor, typename T0, typename T1, typename T, typename ...Ts, template <typename...> class F0, template <typename...> class F1, template <typename...> class F>
    struct fmap<Functor<T0, T1, T, Ts...>, F0, F1, F>
    {
        using type = Functor<typename F0<T0>::type, typename F1<T1>::type, typename F<T>::type, Ts...>;
    };

    template <typename Functor, template <typename...> class ...Fs>
    using fmap_t = typename fmap<Functor, Fs...>::type;


} // meta
} // namespace cat
