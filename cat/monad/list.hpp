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

#include <list>

namespace cat
{
    // list is a monad:
    //

    template <> struct is_monad<std::list> : std::true_type { };

    // list instance:
    //

    template <typename A, typename Fun, typename Ma_, typename A_>
    struct MonadInstance<std::list<A>, Fun, Ma_, A_> final : Monad<std::list>::
    template where<Fun, A, Ma_, A_>
    {
        using B = inner_type_t<std::result_of_t<Fun(A)>>;

        std::list<B>
        mbind(Ma_ && xs, Fun f) override
        {
            std::list<B> ret;
            for(auto & x : xs)
                for(auto & y : f(forward_as<Ma_>(x)))
                    ret.push_back(std::move(y));
            return ret;
        }

        std::list<A>
        mreturn(A_ && a) override
        {
            return {std::forward<A_>(a)};
        }

    };

    // list is a monad_plus:
    //

    template <> struct is_monad_plus<std::list> : std::true_type { };

    // list instance:
    //

    template <typename A, typename Ma_, typename Mb_>
    struct MonadPlusInstance<std::list<A>, Ma_, Mb_> final : MonadPlus<std::list>::
    template where<A, Ma_, Mb_>
    {
        std::list<A>
        mzero() override
        {
            return {};
        }

        std::list<A>
        mplus(Ma_ && a, Mb_ && b) override
        {
            auto r = std::forward<Ma_>(a);
            r.insert(std::end(r),
                     forward_iterator<Mb_>(std::begin(b)),
                     forward_iterator<Mb_>(std::end(b)));
            return r;
        }
    };


} // namespace cat

