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

#include <deque>
#include <cat/monad/monad.hpp>

namespace cat
{
    // deque is a monad:
    //

    template <> struct is_monad<std::deque> : std::true_type { };

    // deque instance:
    //

    template <typename A, typename Fun>
    struct MonadInstance<std::deque, A, Fun> : Monad<std::deque>::Class<Fun, A>
    {
        using B = inner_type_t<decltype(std::declval<Fun>()(std::declval<A>()))>;

        std::deque<B>
        mbind(std::deque<A> xs, Fun f) final
        {
            std::deque<B> ret;
            for(auto & x : xs)
                for(auto & y : f(std::move(x)))
                    ret.push_back(std::move(y));
            return ret;
        }

        std::deque<A>
        mreturn(A a) final
        {
            return {std::move(a)};
        }

    };

} // namespace cat

