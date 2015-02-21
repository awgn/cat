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
#include <cat/type_traits.hpp>

namespace cat
{
    namespace details
    {
        template <typename Fun, typename Functor, size_t N> struct fmap_type;

        template <typename Fun, template <typename ...> class Functor, typename T0>
        struct fmap_type<Fun, Functor<T0>, 0 >
        {
            using type = Functor< std::result_of_t<Fun(T0)> >;
        };

        template <typename Fun, template <typename ...> class Functor, typename T0, typename T1>
        struct fmap_type<Fun, Functor<T0, T1>, 1 >
        {
            using type = Functor< T0, std::result_of_t<Fun(T1)>>;
        };
    }

    //
    // Map function Fun over Functor
    //

    template <typename Fun, typename Functor, size_t N = 0>
    using fmap_type_t = typename details::fmap_type<Fun, std::decay_t<Functor>, N>::type;

    //
    // Type class Functor
    //

    template <template <typename ...> class F>
    struct Functor
    {
        template <typename Fun, typename Type, size_t Order = 0>
        struct _
        {
            virtual auto fmap(Fun fun, Type fa) -> fmap_type_t<Fun, Type, Order> = 0;
        };
    };

    //
    // Functor Instance
    //

    template <template <typename ...> class Functor, typename ...> struct FunctorInstance;

    //
    // fmap method
    //

    template <template <typename ...> class F, typename Fun, typename ...A>
    auto fmap(Fun f, F<A...> const &xs)
    {
        return FunctorInstance<F, Fun, F<A...> const &>{}.fmap(std::move(f), xs);
    }

    template <template <typename ...> class F, typename Fun, typename ...A>
    auto fmap(Fun f, F<A...> &&xs)
    {
        return FunctorInstance<F, Fun, F<A...> &&>{}.fmap(std::move(f), std::move(xs));
    }


    template <template <typename...> class F>
    struct is_functor : std::false_type
    { };


} // namespace cat

