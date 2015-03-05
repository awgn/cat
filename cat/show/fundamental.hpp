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
#include <cat/bits/type.hpp>

#include <string>


namespace cat
{
    //
    // Instances...
    //

    template <>
    struct ShowInstance<short> final : Show<short>
    {
        std::string show(short const &v) override
        {
            return std::to_string(v);
        }
    };

    template <>
    struct ShowInstance<int> final : Show<int>
    {
        std::string show(int const &v) override
        {
            return std::to_string(v);
        }
    };
    template <>
    struct ShowInstance<long int> final : Show<long int>
    {
        std::string show(long int const &v) override
        {
            return std::to_string(v);
        }
    };
    template <>
    struct ShowInstance<long long int> final : Show<long long int>
    {
        std::string show(long long int const &v) override
        {
            return std::to_string(v);
        }
    };

    template <>
    struct ShowInstance<short unsigned> final : Show<short unsigned>
    {
        std::string show(short unsigned const &v) override
        {
            return std::to_string(v);
        }
    };
    template <>
    struct ShowInstance<unsigned> final : Show<unsigned>
    {
        std::string show(unsigned const &v) override
        {
            return std::to_string(v);
        }
    };
    template <>
    struct ShowInstance<long unsigned> final : Show<long unsigned>
    {
        std::string show(long unsigned const &v) override
        {
            return std::to_string(v);
        }
    };
    template <>
    struct ShowInstance<long long unsigned> final : Show<long long unsigned>
    {
        std::string show(long long unsigned const &v) override
        {
            return std::to_string(v);
        }
    };

    template <>
    struct ShowInstance<float> final : Show<float>
    {
        std::string show(const float &v) override
        {
            return std::to_string(v);
        }
    };
    template <>
    struct ShowInstance<double> final : Show<double>
    {
        std::string show(const double &v) override
        {
            return std::to_string(v);
        }
    };
    template <>
    struct ShowInstance<long double> final : Show<long double>
    {
        std::string show(const long double &v) override
        {
            return std::to_string(v);
        }
    };

    template <>
    struct ShowInstance<char> final : Show<char>
    {
        std::string show(char const &c) override
        {
            return "'" + std::string(1, c) + "'";
        }
    };

    template <>
    struct ShowInstance<unsigned char> final : Show<unsigned char>
    {
        std::string show(unsigned char const &c) override
        {
            return std::to_string(c);
        }
    };

    template <>
    struct ShowInstance<bool> final : Show<bool>
    {
        std::string show(bool const &v) override
        {
            return v ? "true" : "false";
        }
    };

    template <>
    struct ShowInstance<std::nullptr_t> final : Show<std::nullptr_t>
    {
        std::string show(std::nullptr_t const &) override
        {
            return "nullptr";
        }
    };

    template <typename T, T value>
    struct ShowInstance<std::integral_constant<T, value>> final : Show<std::integral_constant<T, value>>
    {
        std::string
        show(std::integral_constant<T, value> const &) override
        {
            return type_name<T>() + ":" + cat::show(value);
        }
    };

    template <typename T, T ... Ints>
    struct ShowInstance<std::integer_sequence<T, Ints...>> final : Show<std::integer_sequence<T, Ints...>>
    {
        std::string
        show(std::integer_sequence<T, Ints...> const &) override
        {
            std::string out = "{" + type_name<T>() + ": ";
            if (sizeof...(Ints))
            {
                size_t n = 0;
                do {
                    out += std::to_string(n);
                }
                while ([&]() -> bool {
                    auto cont = ++n < sizeof...(Ints);
                    if (cont)
                        out += ", ";
                    return cont;
                }());
            }
            return out + " }";
        }
    };

}
