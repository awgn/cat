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

#include <cat/monad/monad.hpp>

#include <future>

namespace cat
{
    // std::future is a monad:
    //

    template <> struct is_monad<std::future> : std::true_type { };

    // std::future instance:
    //

    template <typename A, typename Fun, typename Ma_, typename A_>
    struct MonadInstance<std::future<A>, Fun, Ma_, A_> final : Monad<std::future>::
    template where<Fun, A, Ma_, A_>
    {
        using B = inner_type_t<std::result_of_t<Fun(A)>>;

        std::future<B>
        mbind(Ma_ && x, Fun f) override
        {
            return std::async(std::launch::deferred,
                              [=](auto && x)
                              {
                                    return f(x.get()).get();

                              }, std::forward<Ma_>(x));
        }

        std::future<A>
        mreturn(A_ && x) override
        {
            return std::async(std::launch::deferred,
                              [](auto && x) { return x; },
                              std::forward<A_>(x));
        }

    };


} // namespace cat

