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

#include <cat/functional.hpp>
#include <cat/type_traits.hpp>

#include <utility>

namespace cat
{
    //
    // trait for concepts
    //

    template <template <typename...> class BF>
    struct is_bifunctor : std::false_type
    { };

    //
    // type class Bifunctor
    //

    template <template <typename ...> class BF>
    struct Bifunctor
    {
        template <typename A, typename B, typename F, typename G, typename B_>
        struct where
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

    namespace details
    {
        using namespace placeholders;

        struct bimap_
        {
            using function_type = _B<_c,_d>(_f<_c(_a)>, _f<_d(_b)>, _B<_a, _b>);

            template <typename F, typename G, typename B_>
            auto operator()(F f, G g, B_ && xs) const
            {
                static_assert(meta::on_outer_type<is_bifunctor, std::decay_t<B_>>::value, "Type not a bifunctor!");
                return BifunctorInstance<std::decay_t<B_>, F, G, B_>{}.bimap(std::move(f), std::move(g), std::forward<B_>(xs));
            }
        };

        struct bifirst_
        {
            using function_type = _B<_c,_b>(_f<_c(_a)>, _B<_a, _b>);

            template <typename F, typename B_>
            auto operator()(F f, B_ && xs) const
            {
                static_assert(meta::on_outer_type<is_bifunctor, std::decay_t<B_>>::value, "Type not a bifunctor!");
                return BifunctorInstance<std::decay_t<B_>, F, Identity, B_>{}.bimap(std::move(f), identity, std::forward<B_>(xs));
            }
        };

        struct bisecond_
        {
            using function_type = _B<_a,_d>(_f<_d(_b)>, _B<_a, _b>);

            template <typename G, typename B_>
            auto operator()(G g, B_ && xs) const
            {
                static_assert(meta::on_outer_type<is_bifunctor, std::decay_t<B_>>::value, "Type not a bifunctor!");
                return BifunctorInstance<std::decay_t<B_>, Identity, G, B_>{}.bimap(identity, std::move(g), std::forward<B_>(xs));
            }
        };
    }

    constexpr auto bimap = details::bimap_{};
    constexpr auto bifirst = details::bifirst_ {};
    constexpr auto bisecond = details::bisecond_{};

} // namespace cat

