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

#include <cat/utility.hpp>
#include <cat/type_traits.hpp>

namespace cat
{
    //
    // class functor
    //

    template <template <typename ...> class F>
    struct Functor
    {
        template <typename A, typename Fun, typename Type>
        struct _
        {
            virtual auto fmap(Fun fun, Type && fa) -> F<std::result_of_t<Fun(A)>> = 0;
        };

        template <typename K, typename A, typename Fun, typename Type>
        struct _2
        {
            virtual auto fmap(Fun fun, Type && fa) -> F<K, std::result_of_t<Fun(A)>> = 0;
        };

    };

    //
    // instance
    //

    template <typename F, typename ...> struct FunctorInstance;

    //
    // free function
    //

    template <typename Fun, typename Type>
    auto fmap(Fun f, Type && xs)
    {
        return FunctorInstance<std::decay_t<Type>, Fun, Type>{}.fmap(std::move(f), std::forward<Type>(xs));
    }

    //
    // trait for concept
    //

    template <template <typename...> class F>
    struct is_functor : std::false_type
    { };


} // namespace cat

