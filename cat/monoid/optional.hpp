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

#include <experimental/optional>
#include <cat/monoid/monoid.hpp>

namespace cat
{
    template <typename T>
    struct is_monoid<std::experimental::optional<T>> : std::true_type { };

    template <typename T, template <typename ...> class F>
    struct MonoidInstance<std::experimental::optional<T>, F>
        : Monoid<std::experimental::optional<T>>::template Class<F>
    {
        static_assert(is_monoid<T>::value, "optional<T>: T must be a monoid");

        virtual std::experimental::optional<T> mempty() final
        {
            return std::experimental::optional<T>{};
        }

        virtual std::experimental::optional<T>
        mappend(std::experimental::optional<T> const &a,
                std::experimental::optional<T> const &b) final
        {
            if (!a && !b)
                std::experimental::optional<T>{};
            if (a  && !b)
                return a;
            if (!a && b)
                return b;
            return std::experimental::make_optional(cat::mappend(*a, *b));
        }
    };
};