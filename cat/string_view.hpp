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

#include <type_traits>
#include <experimental/string_view>

#include <cctype>
#include <limits>
#include <cstring>

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


} // namespace cat
