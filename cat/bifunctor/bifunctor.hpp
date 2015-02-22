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
#include <cat/functional.hpp>
#include <cat/type_traits.hpp>

namespace cat
{
    //
    // Type class Bifunctor
    //

    template <template <typename ...> class BF>
    struct Bifunctor
    {
        template <typename F, typename G, typename Type>
        struct _
        {
            virtual auto bimap(F f, G g, Type && bf)
                -> map_result_of_t<std::decay_t<Type>, F, G> = 0;

            virtual auto bifirst(F f, Type && bf)
                -> map_result_of_t<std::decay_t<Type>, F, Identity> = 0;

            virtual auto bisecond(G g, Type && bf)
                -> map_result_of_t<std::decay_t<Type>, Identity, G> = 0;
        };
    };

    //
    // Instance
    //

    template <class Bifunctor, typename ...> struct BifunctorInstance;

    //
    // methods
    //

    template <typename F, typename G, typename Type>
    auto bimap(F f, G g, Type && xs)
    {
        return BifunctorInstance<outer_type_t<std::decay_t<Type>>, F, G, Type>{}.bimap(std::move(f), std::move(g), std::forward<Type>(xs));
    }

    template <typename F, typename Type>
    auto bifirst(F f, Type && xs)
    {
        return BifunctorInstance<outer_type_t<std::decay_t<Type>>, F, Identity, Type>{}.bimap(std::move(f), identity, std::forward<Type>(xs));
    }

    template <typename G, typename Type>
    auto bisecond(G g, Type && xs)
    {
        return BifunctorInstance<outer_type_t<std::decay_t<Type>>, Identity, G, Type>{}.bimap(identity, std::move(g), std::forward<Type>(xs));
    }

    //
    // trait for concept
    //

    template <template <typename...> class BF>
    struct is_bifunctor : std::false_type
    { };

} // namespace cat

