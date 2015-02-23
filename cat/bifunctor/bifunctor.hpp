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
    // type class Bifunctor
    //

    template <template <typename ...> class BF>
    struct Bifunctor
    {
        template <typename A, typename B, typename F, typename G, typename Fab_>
        struct _
        {
            virtual auto bimap(F f, G g, Fab_ && bf)
                -> BF< std::result_of_t<F(A)>,
                       std::result_of_t<G(B)>> = 0;
        };
    };

    //
    // instance
    //

    template <class Bifunctor, typename ...> struct BifunctorInstance;

    //
    // free functions
    //

    template <typename F, typename G, typename Fab_>
    auto bimap(F f, G g, Fab_ && xs)
    {
        return BifunctorInstance<std::decay_t<Fab_>, F, G, Fab_>{}.bimap(std::move(f), std::move(g), std::forward<Fab_>(xs));
    }

    template <typename F, typename Fab_>
    auto bifirst(F f, Fab_ && xs)
    {
        return BifunctorInstance<std::decay_t<Fab_>, F, Identity, Fab_>{}.bimap(std::move(f), identity, std::forward<Fab_>(xs));
    }

    template <typename G, typename Fab_>
    auto bisecond(G g, Fab_ && xs)
    {
        return BifunctorInstance<std::decay_t<Fab_>, Identity, G, Fab_>{}.bimap(identity, std::move(g), std::forward<Fab_>(xs));
    }

    //
    // trait for concept
    //

    template <template <typename...> class BF>
    struct is_bifunctor : std::false_type
    { };

} // namespace cat

