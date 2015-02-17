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

#include <list>
#include <cat/monad/monad.hpp>

namespace cat
{
    // list is a monad:
    //

    template <> struct is_monad<std::list> : std::true_type { };

    // list instance:
    //

    template <typename A, typename Fun>
    struct MonadInstance<std::list, A, Fun> : Monad<std::list>::Class<Fun, A>
    {
        using B = inner_type_t<decltype(std::declval<Fun>()(std::declval<A>()))>;

        std::list<B>
        mbind(std::list<A> const &xs, Fun f) final
        {
            std::list<B> ret;
            for(auto & x : xs)
                for(auto &y : f(x))
                    ret.push_back(std::move(y));
            return ret;
        }

        std::list<A>
        mreturn(A const &a) final
        {
            return {a};
        }
    };

} // namespace cat

