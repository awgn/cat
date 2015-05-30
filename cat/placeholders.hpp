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

namespace cat
{
    namespace placeholders
    {
        struct unspec { };

        struct __ { };

        struct _a { };
        struct _b { };
        struct _c { };
        struct _d { };
        struct _e { };
        struct _g { };
        struct _h { };
        struct _i { };
        struct _j { };
        struct _k { };
        struct _l { };
        struct _m { };                                      // monoid
        struct _n { };
        struct _o { };
        struct _p { };
        struct _q { };
        struct _r { };
        struct _s { };
        struct _t { };
        struct _u { };
        struct _v { };
        struct _w { };
        struct _x { };
        struct _y { };
        struct _z { };

        template <typename T> struct _f { };               // generic function: _f<_b(_a)>  => f :: a -> b

        template <typename T> struct _C { };                // container
        template <typename T> struct _F { };                // functor
        template <typename T> struct _M { };                // monad
        template <typename T, typename V> struct _B { };    // bifunctor
    };

    constexpr auto _ = placeholders::__ {};

} // namespace cat

