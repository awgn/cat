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

#include <map>
#include <algorithm>

#include <cat/monoid/monoid.hpp>

namespace cat
{
    template <typename K, typename V>
    struct is_monoid<std::map<K,V>> : std::true_type { };

    template <typename K, typename V, template <typename ...> class F>
    struct MonoidInstance<std::map<K,V>, F> : Monoid<std::map<K,V>>::template Class<F>
    {
        virtual std::map<K,V> mempty() final
        {
            return std::map<K,V>{};
        }

        virtual std::map<K,V> mappend(std::map<K,V> const &a, std::map<K,V> const &b) final
        {
            auto ret = a;
            ret.insert(std::begin(b), std::end(b));
            return ret;
        }
    };
};
