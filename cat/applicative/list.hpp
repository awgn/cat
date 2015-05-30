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

#include <list>

namespace cat
{
    // list is an applicative instance:
    //

    template <> struct is_applicative<std::list> : std::true_type { };

    // list instance:
    //

    template <typename F, typename A, typename Ff_, typename Fa_, typename A_>
    struct ApplicativeInstance<std::list<F>, std::list<A>, Ff_, Fa_, A_>  final : Applicative<std::list>::
    template where<F, A, Ff_, Fa_, A_>
    {
        using B = std::result_of_t<F(A_)>;

        std::list<A>
        pure(A_ &&elem) override
        {
            return { std::forward<A_>(elem) };
        }

        std::list<B>
        apply(Ff_ && fs, Fa_ &&xs) override
        {
            std::list<B> out;

            for(auto &f : std::forward<Ff_>(fs))
                for(auto &x : std::forward<Fa_>(xs))
                    out.push_back(forward_as<Ff_>(f)(forward_as<Fa_>(x)));

            return out;
        }
    };

    // list is an alternative instance:
    //

    template <> struct is_alternative<std::list> : std::true_type { };

    // list instance:
    //

    template <typename A, typename Fl_, typename Fr_>
    struct AlternativeInstance<std::list<A>, Fl_, Fr_> final : Alternative<std::list>::
    template where<std::list<A>, Fl_, Fr_>
    {
        std::list<A>
        empty() override
        {
            return std::list<A>{};
        }

        std::list<A>
        or_(Fl_ && lhs, Fr_ && rhs) override
        {
            std::list<A> ret = std::forward<Fl_>(lhs);

            ret.insert(std::end(ret),
                        forward_iterator<Fr_>(std::begin(rhs)),
                        forward_iterator<Fr_>(std::end(rhs))
                      );
            return ret;
        }
    };

} // namespace cat

