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

#include <cat/optional.hpp>
#include <cat/monoid/monoid.hpp>

namespace cat
{
    template <typename T>
    struct is_monoid<optional<T>> : std::true_type { };

    template <typename F, typename M1, typename M2, typename T>
    struct MonoidInstance<optional<T>, F, M1, M2> final : Monoid<optional<T>>::
    template where<F, M1, M2>
    {
        static_assert(is_monoid<T>::value, "optional<T>: T must be a monoid");

        virtual optional<T> mempty() override
        {
            return optional<T>{};
        }

        virtual optional<T>
        mappend(M1 &&a, M2 &&b) override
        {
            if (a) {
                return b ? make_optional(cat::mappend(forward_as<M1>(*a), forward_as<M2>(*b)))
                         : a;
            }
            return b;
        }
    };
};
