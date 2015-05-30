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

#include <cat/__config.hpp>

#include <iterator>
#include <ciso646>


//
// The date 20150119 is learned from ranges-v3 library (as it performs a similar test)...
//

#if (defined(_LIBCPP_VERSION) && (_LIBCPP_VERSION > 1101)) || \
    ( defined(__GLIBCXX__) && (__GLIBCXX__ < 20150119))

namespace std
{
    template <class Tp, size_t Np>
    inline reverse_iterator<Tp*>
    rbegin(Tp (&__array)[Np])
    {
        return reverse_iterator<Tp*>(__array + Np);
    }

    template <class Tp, size_t Np>
    inline reverse_iterator<Tp*>
    rend(Tp (&__array)[Np])
    {
        return reverse_iterator<Tp*>(__array);
    }

    template <class Ep>
    inline reverse_iterator<const Ep*>
    rbegin(initializer_list<Ep> il)
    {
        return reverse_iterator<const Ep*>(il.end());
    }

    template <class Ep>
    inline reverse_iterator<const Ep*>
    rend(initializer_list<Ep> il)
    {
        return reverse_iterator<const Ep*>(il.begin());
    }

    template <class Cp>
    inline auto
    rbegin(Cp& c) -> decltype(c.rbegin())
    {
        return c.rbegin();
    }

    template <class Cp>
    inline auto
    rbegin(const Cp& c) -> decltype(c.rbegin())
    {
        return c.rbegin();
    }

    template <class Cp>
    inline auto
    rend(Cp& c) -> decltype(c.rend())
    {
        return c.rend();
    }

    template <class Cp>
    inline auto
    rend(const Cp& c) -> decltype(c.rend())
    {
        return c.rend();
    }


    template <class Cp>
    inline auto
    crbegin(const Cp& c) -> decltype(rbegin(c))
    {
        return rbegin(c);
    }

    template <class Cp>
    inline auto
    crend(const Cp& c) -> decltype(rend(c))
    {
        return rend(c);
    }
}

#endif

