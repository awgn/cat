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
#include <cat/traits.hpp>

namespace cat
{
    //
    // class Functor
    //

    template <template <typename ...> class F>
    struct Functor
    {
        template <typename Fun, typename A, typename ...Args>
        struct Class
        {
            virtual auto fmap(Fun f, F<A, Args...> const & fa)
                -> F< decltype(f(std::declval<A>())), rebind_t<Args, decltype(f(std::declval<A>()))>...> = 0;
        };

        template <typename Fun, typename K, typename A, typename ...Args>
        struct Class1
        {
            virtual auto fmap(Fun f, F<K, A, Args...> const & fa)
                -> F<K, decltype(f(std::declval<A>())), rebind_t<Args, std::pair<const K, decltype(f(std::declval<A>()))>>...> = 0;
        };

    };

    template <template <typename ...> class F, typename ...> struct FunctorInstance;

    template <template <typename ...> class F, typename Fun, typename ...A>
    auto fmap(Fun f, F<A...> const &xs)
    {
        return FunctorInstance<F, Fun, A...>{}.fmap(f, xs);
    }

    template <template <typename...> class F>
    struct is_functor : std::false_type
    { };

} // namespace cat

