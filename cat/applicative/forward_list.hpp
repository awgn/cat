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

#include <forward_list>

namespace cat
{
    // forward_list is an applicative instance:
    //

    template <> struct is_applicative<std::forward_list> : std::true_type { };

    // forward_list instance:
    //

    template <typename F, typename A, typename Ff_, typename Fa_, typename A_>
    struct ApplicativeInstance<std::forward_list<F>, std::forward_list<A>, Ff_, Fa_, A_>  final : Applicative<std::forward_list>::
    template where<F, A, Ff_, Fa_, A_>
    {
        using B = std::result_of_t<F(A_)>;

        std::forward_list<A>
        pure(A_ &&elem) override
        {
            return { std::forward<A_>(elem) };
        }

        std::forward_list<B>
        apply(Ff_ && fs, Fa_ &&xs) override
        {
            std::forward_list<B> out;

            for(auto &f : std::forward<Ff_>(fs))
                for(auto &x : std::forward<Fa_>(xs))
                    out.push_front(forward_as<Ff_>(f)(forward_as<Fa_>(x)));

            out.reverse();
            return out;
        }

    };

    // forward_list is an alternative instance:
    //

    template <> struct is_alternative<std::forward_list> : std::true_type { };

    // forward_list instance:
    //

    template <typename A, typename Fl_, typename Fr_>
    struct AlternativeInstance<std::forward_list<A>, Fl_, Fr_> final : Alternative<std::forward_list>::
    template where<std::forward_list<A>, Fl_, Fr_>
    {
        std::forward_list<A>
        empty() override
        {
            return std::forward_list<A>{};
        }

        std::forward_list<A>
        or_(Fl_ && lhs, Fr_ && rhs) override
        {
            std::forward_list<A> ret = std::forward<Fl_>(lhs);
            auto before_end = ret.before_begin();
            for(auto &e : ret) {
                (void)e;
                ++before_end;
            }
            ret.insert_after(before_end,
                        forward_iterator<Fr_>(std::begin(rhs)),
                        forward_iterator<Fr_>(std::end(rhs))
                      );
            return ret;
        }
    };
} // namespace cat

