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

#include <cat/show/show.hpp>
#include <cat/type_traits.hpp>
#include <cat/string_view.hpp>

#include <string>

namespace cat
{
    //
    // Instances...
    //

    template <typename T>
    struct ShowInstance<std::basic_string<T>> final : Show<std::basic_string<T>>
    {
        std::string
        show(std::basic_string<T> const &str) override
        {
            std::string ret("[ ");
            for(auto e : str)
                ret += ::cat::show(e) + " ";
            return ret + ']';
        }
    };

    template <>
    struct ShowInstance<std::string> final : Show<std::string>
    {
        std::string show(std::string const &v) override
        {
            return '"' + v + '"';
        }
    };

    template <>
    struct ShowInstance<string_view> final : Show<string_view>
    {
        std::string show(string_view const &v) override
        {
#ifdef _LIBCPP_VERSION
            //FIXME: string_view::to_string is not marked const in some libc++ versions.
            return '"' + const_cast<string_view &>(v).to_string() + '"';
#else
            return '"' + v.to_string() + '"';
#endif
        }
    };

    template <>
    struct ShowInstance<const char *> final : Show<const char *>
    {
        std::string show(const char * const & s) override
        {
            if (s)
                return '"' + std::string(s) + '"';
            return "NULL";
        }
    };

    template <size_t N>
    struct ShowInstance<char[N]> final : Show<char[N]>
    {
        using litstring = char[N];
        std::string show(const litstring &s) override
        {
            return '"' + std::string{s} + '"';
        }
    };
}
