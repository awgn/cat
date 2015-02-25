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
    using namespace placeholders;

    //
    // type class Bifunctor
    //

    template <template <typename ...> class BF>
    struct Bifunctor
    {
        template <typename A, typename B, typename F, typename G, typename B_>
        struct _
        {
            virtual auto bimap(F f, G g, B_ && bf)
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

    struct bimap_
    {
        using function_type = _B<_c,_d>(_<_c(_a)>, _<_d(_b)>, _B<_a, _b> &&);

        template <typename F, typename G, typename B_>
        auto operator()(F f, G g, B_ && xs) const
        {
            return BifunctorInstance<std::decay_t<B_>, F, G, B_>{}.bimap(std::move(f), std::move(g), std::forward<B_>(xs));
        }

    } constexpr bimap = bimap_{};

    struct bifirst_
    {
        using function_type = _B<_c,_b>(_<_c(_a)>, _B<_a, _b> &&);

        template <typename F, typename B_>
        auto operator()(F f, B_ && xs) const
        {
            return BifunctorInstance<std::decay_t<B_>, F, Identity, B_>{}.bimap(std::move(f), identity, std::forward<B_>(xs));
        }

    } constexpr bifirst = bifirst_ {};

    struct bisecond_
    {
        using function_type = _B<_a,_d>(_<_d(_b)>, _B<_a, _b> &&);

        template <typename G, typename B_>
        auto operator()(G g, B_ && xs) const
        {
            return BifunctorInstance<std::decay_t<B_>, Identity, G, B_>{}.bimap(identity, std::move(g), std::forward<B_>(xs));
        }

    } constexpr bisecond = bisecond_{};

    //
    // trait for concept
    //

    template <template <typename...> class BF>
    struct is_bifunctor : std::false_type
    { };

} // namespace cat

