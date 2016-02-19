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
#include <cat/bits/type.hpp>

#include <string>


namespace cat
{
    //
    // Instances...
    //

    template <>
    struct PrettyInstance<short> final : Pretty<short>
    {
        std::string pretty(short const &v) override
        {
            return "short{" + std::to_string(v) + '}';
        }
    };

    template <>
    struct PrettyInstance<int> final : Pretty<int>
    {
        std::string pretty(int const &v) override
        {
            return "int{" + std::to_string(v) + '}';
        }
    };
    template <>
    struct PrettyInstance<long int> final : Pretty<long int>
    {
        std::string pretty(long int const &v) override
        {
            return "long int{" + std::to_string(v) + '}';
        }
    };
    template <>
    struct PrettyInstance<long long int> final : Pretty<long long int>
    {
        std::string pretty(long long int const &v) override
        {
            return "long long int{" + std::to_string(v) + '}';
        }
    };

    template <>
    struct PrettyInstance<short unsigned> final : Pretty<short unsigned>
    {
        std::string pretty(short unsigned const &v) override
        {
            return "unsigned short{" + std::to_string(v) + '}';
        }
    };
    template <>
    struct PrettyInstance<unsigned> final : Pretty<unsigned>
    {
        std::string pretty(unsigned const &v) override
        {
            return "unsigned{" + std::to_string(v) + '}';
        }
    };
    template <>
    struct PrettyInstance<long unsigned> final : Pretty<long unsigned>
    {
        std::string pretty(long unsigned const &v) override
        {
            return "unsigned long{" + std::to_string(v) + '}';
        }
    };
    template <>
    struct PrettyInstance<long long unsigned> final : Pretty<long long unsigned>
    {
        std::string pretty(long long unsigned const &v) override
        {
            return "unsigned long long{" + std::to_string(v) + '}';
        }
    };

    template <>
    struct PrettyInstance<float> final : Pretty<float>
    {
        std::string pretty(const float &v) override
        {
            return "float{" + std::to_string(v) + '}';
        }
    };
    template <>
    struct PrettyInstance<double> final : Pretty<double>
    {
        std::string pretty(const double &v) override
        {
            return "double{" + std::to_string(v) + '}';
        }
    };
    template <>
    struct PrettyInstance<long double> final : Pretty<long double>
    {
        std::string pretty(const long double &v) override
        {
            return "long double{" + std::to_string(v) + '}';
        }
    };

    template <>
    struct PrettyInstance<char> final : Pretty<char>
    {
        std::string pretty(char const &c) override
        {
            return "char{'" + std::string(1, c) + "'}";
        }
    };

    template <>
    struct PrettyInstance<unsigned char> final : Pretty<unsigned char>
    {
        std::string pretty(unsigned char const &c) override
        {
            return "unsigned char{" +  std::to_string(c) + '}';
        }
    };

    template <>
    struct PrettyInstance<bool> final : Pretty<bool>
    {
        std::string pretty(bool const &v) override
        {
            return std::string{"bool{"} + (v ? "true" : "false") + '}';
        }
    };

    template <>
    struct PrettyInstance<std::nullptr_t> final : Pretty<std::nullptr_t>
    {
        std::string pretty(std::nullptr_t const &) override
        {
            return "nullptr";
        }
    };

    template <typename T, T value>
    struct PrettyInstance<std::integral_constant<T, value>> final : Pretty<std::integral_constant<T, value>>
    {
        std::string
        pretty(std::integral_constant<T, value> const &) override
        {
            return "integral_constant<" + type_name<T>() + ">:" + cat::pretty(value);
        }
    };

    template <typename T, T ... Ints>
    struct PrettyInstance<std::integer_sequence<T, Ints...>> final : Pretty<std::integer_sequence<T, Ints...>>
    {
        std::string
        pretty(std::integer_sequence<T, Ints...> const &) override
        {
            std::string out = "integer_sequence<" + type_name<T>() + ">{";
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
