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

#include <deque>

namespace cat
{
    // deque is an applicative instance:
    //

    template <> struct is_applicative<std::deque> : std::true_type { };

    // deque instance:
    //

    template <typename F, typename A, typename Ff_, typename Fa_, typename A_>
    struct ApplicativeInstance<std::deque<F>, std::deque<A>, Ff_, Fa_, A_>  final : Applicative<std::deque>::
    template where<F, A, Ff_, Fa_, A_>
    {
        using B = std::result_of_t<F(A_)>;

        std::deque<A>
        pure(A_ &&elem) override
        {
            return { std::forward<A_>(elem) };
        }

        std::deque<B>
        apply(Ff_ && fs, Fa_ && xs) override
        {
            std::deque<B> out;

            for(auto &f : std::forward<Ff_>(fs))
                for(auto &x : std::forward<Fa_>(xs))
                    out.push_back(forward_as<Ff_>(f)(forward_as<Fa_>(x)));

            return out;
        }
    };

    // deque is an alternative instance:
    //

    template <> struct is_alternative<std::deque> : std::true_type { };

    // deque instance:
    //

    template <typename A, typename Fl_, typename Fr_>
    struct AlternativeInstance<std::deque<A>, Fl_, Fr_> final : Alternative<std::deque>::
    template where<std::deque<A>, Fl_, Fr_>
    {
        std::deque<A>
        empty() override
        {
            return std::deque<A>{};
        }

        std::deque<A>
        or_(Fl_ && lhs, Fr_ && rhs) override
        {
            std::deque<A> ret = std::forward<Fl_>(lhs);

            ret.insert(std::end(ret),
                        forward_iterator<Fr_>(std::begin(rhs)),
                        forward_iterator<Fr_>(std::end(rhs))
                      );
            return ret;
        }
    };

} // namespace cat

