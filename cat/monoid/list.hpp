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

#include <cat/monoid/monoid.hpp>
#include <cat/iterator.hpp>

#include <list>

namespace cat
{
    template <typename T>
    struct is_monoid<std::list<T>> : std::true_type { };

    template <typename F, typename M1, typename M2, typename T>
    struct MonoidInstance<std::list<T>, F, M1, M2> final : Monoid<std::list<T>>::
    template where<F, M1, M2>
    {
        virtual std::list<T> mempty() override
        {
            return std::list<T>{};
        }

        virtual std::list<T> mappend(M1 && a, M2 && b) override
        {
            auto ret = std::forward<M1>(a);

            ret.insert(std::end(ret),
                       forward_iterator<M2>(std::begin(b)),
                       forward_iterator<M2>(std::end(b)));

            return ret;
        }
    };

};
