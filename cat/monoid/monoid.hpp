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

#include <vector>
#include <cat/type_traits.hpp>
#include <cat/utility.hpp>

namespace cat
{
    //
    // type class Monoid
    //

    template <typename M1, typename M2>
    auto mappend(M1 && a, M2 && b);

    template <typename M>
    struct Monoid
    {
        template <typename  F, typename M1, typename M2>
        struct _
        {
            virtual M mempty() = 0;
            virtual M mappend(M1 &&, M2 &&) = 0;
            virtual M mconcat(F && xs)
            {
                auto r = mempty();

                for(auto && x : std::forward<F>(xs))
                    r = cat::mappend(std::move(r), forward_as<F>(x));

                return r;
            };
        };
    };

    //
    // instance
    //

    template <typename M, typename F, typename M1, typename M2> struct MonoidInstance;

    //
    // free functions:
    //

    template <typename M>
    auto mempty()
    {
        return MonoidInstance<M, std::vector<M>, M, M>{}.mempty();
    }

    template <typename M1, typename M2>
    auto mappend(M1 && a, M2 && b)
    {
        using M = std::decay_t<M1>;

        return MonoidInstance<
                M, std::vector<M>, M1, M2>{}.
                mappend(std::forward<M1>(a),
                        std::forward<M2>(b));
    }

    template <typename F>
    auto mconcat(F && f)
    {
        using M = inner_type_t<std::decay_t<F>>;

        return MonoidInstance<
                M, F, M, M >{}.
                mconcat(std::forward<F>(f));
    }


    //
    // trait for concept
    //

    template <typename M>
    struct is_monoid : std::false_type
    { };


} // namespace cat

