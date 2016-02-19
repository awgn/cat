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

#include <cat/pretty/pretty.hpp>
#include <cat/placeholders.hpp>
#include <cat/type_traits.hpp>
#include <cat/bits/type.hpp>
#include <cat/existential.hpp>

#include <string>

namespace cat
{
    //
    // instance
    //

    template <typename T> struct PrettyInstance;

    //
    // trait for concepts
    //

    template <typename T>
    struct is_prettyable : has_specialization<PrettyInstance, T>
    { };

    //
    // type class Pretty
    //

    template <typename T>
    struct Pretty
    {
        virtual std::string pretty(T const &) = 0;
    };

    //
    // free function
    //

    template <typename T>
    std::string pretty(T const &v)
    {
        static_assert(is_prettyable<T>::value, "T is not prettyable!");
        return PrettyInstance<T>{}.pretty(v);
    }

#if (__GNUC__ >= 5)
    //
    // overloading for existential quantification...
    //

    template <template <typename ...> class ...TC>
    std::string pretty(forall<TC...> const &value)
    {
        return pretty(static_cast<const forall_1<Pretty> &>(value));
    }
#endif
}
