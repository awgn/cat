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

#include <cat/type_traits.hpp>
#include <cat/bits/type.hpp>
#include <cat/string_view.hpp>
#include <cat/optional.hpp>

namespace cat
{
    //
    // instance
    //

    template <typename T> struct ReadInstance;

    //
    // trait for concepts
    //

    template <typename T>
    struct is_readable : has_specialization<ReadInstance, T>
    { };

    //
    // type class Read
    //

    template <typename T>
    struct Read
    {
        virtual optional<std::pair<T, string_view>> reads(string_view) = 0;
    };

    //
    // free function
    //

    template <typename T>
    T read(string_view str)
    {
        static_assert(is_readable<T>::value, "T is not readable!");
        auto r = ReadInstance<T>{}.reads(str);
        if (!r)
            throw std::runtime_error(type_name<T>() + ": read error!");
        return r.value().first;
    }

    template <typename T>
    T read(std::string const &str)
    {
        static_assert(is_readable<T>::value, "T is not readable!");
        return read<T>(string_view(str));
    }

    template <typename T>
    T read(const char *str)
    {
        static_assert(is_readable<T>::value, "T is not readable!");
        return read<T>(string_view(str));
    }

    //
    // reads
    //

    template <typename T>
    optional<std::pair<T, string_view>>
    reads(std::string const &str)
    {
        static_assert(is_readable<T>::value, "T is not readable!");
        return ReadInstance<T>{}.reads(string_view(str));
    }

    template <typename T>
    optional<std::pair<T, string_view>>
    reads(string_view str)
    {
        static_assert(is_readable<T>::value, "T is not readable!");
        return ReadInstance<T>{}.reads(str);
    }

    template <typename T>
    optional<std::pair<T, string_view>>
    reads(const char *str)
    {
        static_assert(is_readable<T>::value, "T is not readable!");
        return ReadInstance<T>{}.reads(string_view{str});
    }

}
