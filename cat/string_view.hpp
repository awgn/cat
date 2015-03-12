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

#include <cat/bits/type.hpp>
#include <cat/optional.hpp>

#include <type_traits>
#include <experimental/string_view>

#include <cctype>
#include <vector>
#include <limits>
#include <cstring>
#include <stdexcept>

namespace cat
{
    using std::experimental::string_view;

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
    }

    //
    // to_number: takes a string_view, return a pair<Number, string_view>
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
        T r = details::to_number<T>(str, &end, base, std::is_floating_point<T>{}, std::is_signed<T>{});
        v.remove_prefix(end - str);
        return { r, v };
    }

    //
    // skipws: consume white spaces.
    //

    inline
    string_view skipws(string_view s)
    {
        while(!s.empty() &&
              std::isspace(s.front()))
            s.remove_prefix(1);
        return s;
    }

    //
    // consume the given char, return nullopt
    // otherwise
    //

    inline optional<string_view>
    consume(const char c, string_view s)
    {
        auto s1 = skipws(s);
        if (!s1.empty() && s1.front() == c)
        {
            s1.remove_prefix(1);
            return s1;
        }
        return nullopt;
    }

    //
    // consume the given string, return nullopt
    // otherwise
    //

    inline optional<string_view>
    consume(const char *str, string_view s)
    {
        auto size = std::strlen(str);
        auto s1 = skipws(s);
        if (s1.compare(0, size, str))
            return nullopt;
        s1.remove_prefix(size);
        return s1;
    }

    //
    // trim a string_view
    //

    inline string_view
    trim(string_view str)
    {
        auto b = str.find_first_not_of(" \n\r\t");
        auto e = str.find_last_not_of(" \n\r\t");

        b = b == string_view::npos ? 0 : b;
        e = e == string_view::npos ? 0 : (e + 1 - b);

        return str.substr(b, e);
    }

    //
    // split a string_view by means of the given separator.
    //

    inline std::vector<std::string>
    split(string_view str, const char *sep)
    {
        std::vector<std::string> ret;
        auto len = std::strlen(sep);

        for(std::string::size_type n; (n = str.find(sep)) != std::string::npos;)
        {
            ret.push_back(std::string(str.substr(0,n)));
            str = str.substr(n + len, std::string::npos);
        }

        ret.push_back(std::string(str));
        return ret;
    }

} // namespace cat
