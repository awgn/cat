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

#include <cat/applicative/applicative.hpp>

#include <future>

namespace cat
{
    // std::future is an applicative instance:
    //

    template <> struct is_applicative<std::future> : std::true_type { };

    // std::future instance:
    //

    template <typename F, typename A, typename Ff_, typename Fa_, typename A_>
    struct ApplicativeInstance<std::future<F>, std::future<A>, Ff_, Fa_, A_>  final : Applicative<std::future>::
    template where<F, A, Ff_, Fa_, A_>
    {
        using B = std::result_of_t<F(A_)>;

        std::future<A>
        pure(A_ &&elem) override
        {
            return std::async(std::launch::deferred,
                              [](auto && x) { return x; },
                              std::forward<A_>(elem));
        }

        std::future<B>
        apply(Ff_ && f, Fa_ && x) override
        {
            return std::async(std::launch::deferred,
                              [](std::decay_t<Ff_> f, std::decay_t<Fa_> x)
                              {
                                    auto f_ = f.get();
                                    auto x_ = x.get();
                                    return f_(std::move(x_));
                              },
                              std::forward<Ff_>(f), std::forward<Fa_>(x));
        }

    };


} // namespace cat

