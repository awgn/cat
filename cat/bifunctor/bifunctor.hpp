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
    //
    // class Bifunctor
    //

    template <template <typename ...> class BF>
    struct Bifunctor
    {
        template <typename F, typename G, typename A, typename B>
        struct Class
        {
            virtual auto bimap(F f, G g, BF<A, B> const & bf)
                -> BF<decltype(f(std::declval<A>())),
                      decltype(g(std::declval<B>()))> = 0;

            virtual auto first(F f, BF<A, B> const & bf)
                -> BF<decltype(f(std::declval<A>())), B> = 0;

            virtual auto second(G g, BF<A, B> const & bf)
                -> BF<A, decltype(g(std::declval<B>()))> = 0;
        };
    };

    template <template <typename ...> class BF, typename ...> struct BifunctorInstance;

    template <template <typename ...> class BF, typename F, typename G, typename A, typename B>
    auto bimap(F f, G g, BF<A,B> const &xs)
    {
        return BifunctorInstance<BF, F, G, A, B>{}.bimap(f, g, xs);
    }

    template <template <typename ...> class BF, typename F, typename A, typename B>
    auto first(F f, BF<A,B> const &xs)
    {
        return BifunctorInstance<BF, F, F, A, B>{}.first(f, xs);
    }

    template <template <typename ...> class BF, typename G, typename A, typename B>
    auto second(G g, BF<A,B> const &xs)
    {
        return BifunctorInstance<BF, G, G, A, B>{}.second(g, xs);
    }

    template <template <typename...> class BF>
    struct is_bifunctor : std::false_type
    { };

} // namespace cat

