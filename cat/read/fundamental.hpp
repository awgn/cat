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

#include <cat/read/read.hpp>
#include <cat/bits/type.hpp>

#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <limits>
#include <utility>

namespace cat
{
    namespace details
    {
        // numbers...
        //

        template <typename T, typename V> void check_limits(V value)
        {
            if (value > std::numeric_limits<T>::max() ||
                value < std::numeric_limits<T>::min())
                throw std::runtime_error("to_number: " + type_name<T>());
        }

        template <typename T>
        T to_number(const char *str, char **end, int base, std::false_type, std::true_type)
        {
            long long n = std::strtoll(str, end, base);
            if (str != *end)
                check_limits<T>(n);
            return n;
        }

        template <typename T>
        T to_number(const char *str, char **end, int base, std::false_type, std::false_type)
        {
            unsigned long long n = std::strtoull(str, end, base);
            if (str != *end)
                check_limits<T>(n);
            return n;
        }

        template <typename T>
        T to_number(const char *str, char **end, int, std::true_type, std::true_type)
        {
            long double n = std::strtold(str, end);
            if (str != *end)
                check_limits<T>(n);
            return n;
        }

        //
        // to_number: takes a string_view, return a pair<T, string_view>
        //

        template <typename T>
        std::pair<T, string_view>
        to_number(string_view v, int base = 10)
        {
            auto size = v.size();
            char str[size+1];
            str[size] = '\0';
            memcpy(str, v.data(), size);

            char *end;
            T r = to_number<T>(str, &end, base, std::is_floating_point<T>{}, std::is_signed<T>{});
            v.remove_prefix(end - str);
            return { r, v };
        }
    }

    //
    // Instances...
    //

    template <>
    struct ReadInstance<short> final : Read<short>
    {
        std::experimental::optional<std::pair<short,string_view>> reads(string_view v)
        {
            auto ret = details::to_number<short>(v);
            if (ret.second == v)
                return std::experimental::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<int> final : Read<int>
    {
        std::experimental::optional<std::pair<int,string_view>> reads(string_view v)
        {
            auto ret = details::to_number<int>(v);
            if (ret.second == v)
                return std::experimental::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<long int> final : Read<long int>
    {
        std::experimental::optional<std::pair<long int,string_view>> reads(string_view v)
        {
            auto ret = details::to_number<long int>(v);
            if (ret.second == v)
                return std::experimental::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<long long int> final : Read<long long int>
    {
        std::experimental::optional<std::pair<long long int,string_view>> reads(string_view v)
        {
            auto ret = details::to_number<long long int>(v);
            if (ret.second == v)
                return std::experimental::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<unsigned short> final : Read<unsigned short>
    {
        std::experimental::optional<std::pair<unsigned short,string_view>> reads(string_view v)
        {
            auto ret = details::to_number<unsigned short>(v);
            if (ret.second == v)
                return std::experimental::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<unsigned> final : Read<unsigned>
    {
        std::experimental::optional<std::pair<unsigned,string_view>> reads(string_view v)
        {
            auto ret = details::to_number<unsigned>(v);
            if (ret.second == v)
                return std::experimental::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<long unsigned> final : Read<long unsigned>
    {
        std::experimental::optional<std::pair<long unsigned,string_view>> reads(string_view v)
        {
            auto ret = details::to_number<long unsigned>(v);
            if (ret.second == v)
                return std::experimental::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<long long unsigned> final : Read<long long unsigned>
    {
        std::experimental::optional<std::pair<long long unsigned,string_view>> reads(string_view v)
        {
            auto ret = details::to_number<long long unsigned>(v);
            if (ret.second == v)
                return std::experimental::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<float> final : Read<float>
    {
        std::experimental::optional<std::pair<float,string_view>> reads(string_view v)
        {
            auto ret = details::to_number<float>(v);
            if (ret.second == v)
                return std::experimental::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<double> final : Read<double>
    {
        std::experimental::optional<std::pair<double,string_view>> reads(string_view v)
        {
            auto ret = details::to_number<double>(v);
            if (ret.second == v)
                return std::experimental::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<long double> final : Read<long double>
    {
        std::experimental::optional<std::pair<long double,string_view>> reads(string_view v)
        {
            auto ret = details::to_number<long double>(v);
            if (ret.second == v)
                return std::experimental::nullopt;
            return ret;
        }
    };


    template <>
    struct ReadInstance<char> final : Read<char>
    {
        std::experimental::optional<std::pair<char,string_view>> reads(string_view v)
        {
            auto it = std::begin(v);
            for(; it != std::end(v); ++it)
                if (!std::isspace(*it))
                    break;

            if (it == std::end(v))
                return std::experimental::nullopt;

            v.remove_prefix(1 + std::distance(std::begin(v), it));
            return std::make_pair(*it, v);
        }
    };

    template <>
    struct ReadInstance<unsigned char> final : Read<unsigned char>
    {
        std::experimental::optional<std::pair<unsigned char,string_view>> reads(string_view v)
        {
            auto it = std::begin(v);
            for(; it != std::end(v); ++it)
                if (!std::isspace(*it))
                    break;

            if (it == std::end(v))
                return std::experimental::nullopt;

            v.remove_prefix(1 + std::distance(std::begin(v), it));
            return std::make_pair(static_cast<unsigned char>(*it), v);
        }
    };

    template <>
    struct ReadInstance<bool> final : Read<bool>
    {
        std::experimental::optional<std::pair<bool,string_view>> reads(string_view v)
        {
            std::istringstream in(v.to_string());
            bool x; in >> std::noboolalpha;
            if (!(in >> x)) {
                in.clear();
                if (!(in >> std::boolalpha >> x))
                    return std::experimental::nullopt;
            }
            v.remove_prefix(in.tellg());
            return std::make_pair(x, v);
        }
    };

}
