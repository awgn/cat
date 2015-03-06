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
#include <utility>

namespace cat
{
    //
    // Instances...
    //

    template <>
    struct ReadInstance<short> final : Read<short>
    {
        optional<std::pair<short,string_view>>
        reads(string_view v) override
        {
            auto ret = to_number<short>(v);
            if (ret.second == v)
                return nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<int> final : Read<int>
    {
        optional<std::pair<int,string_view>>
        reads(string_view v) override
        {
            auto ret = to_number<int>(v);
            if (ret.second == v)
                return nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<long int> final : Read<long int>
    {
        optional<std::pair<long int,string_view>>
        reads(string_view v) override
        {
            auto ret = to_number<long int>(v);
            if (ret.second == v)
                return nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<long long int> final : Read<long long int>
    {
        optional<std::pair<long long int,string_view>>
        reads(string_view v) override
        {
            auto ret = to_number<long long int>(v);
            if (ret.second == v)
                return nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<unsigned short> final : Read<unsigned short>
    {
        optional<std::pair<unsigned short,string_view>>
        reads(string_view v) override
        {
            auto ret = to_number<unsigned short>(v);
            if (ret.second == v)
                return nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<unsigned> final : Read<unsigned>
    {
        optional<std::pair<unsigned,string_view>>
        reads(string_view v) override
        {
            auto ret = to_number<unsigned>(v);
            if (ret.second == v)
                return nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<long unsigned> final : Read<long unsigned>
    {
        optional<std::pair<long unsigned,string_view>>
        reads(string_view v) override
        {
            auto ret = to_number<long unsigned>(v);
            if (ret.second == v)
                return nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<long long unsigned> final : Read<long long unsigned>
    {
        optional<std::pair<long long unsigned,string_view>>
        reads(string_view v) override
        {
            auto ret = to_number<long long unsigned>(v);
            if (ret.second == v)
                return nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<float> final : Read<float>
    {
        optional<std::pair<float,string_view>>
        reads(string_view v) override
        {
            auto ret = to_number<float>(v);
            if (ret.second == v)
                return nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<double> final : Read<double>
    {
        optional<std::pair<double,string_view>>
        reads(string_view v) override
        {
            auto ret = to_number<double>(v);
            if (ret.second == v)
                return nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<long double> final : Read<long double>
    {
        optional<std::pair<long double,string_view>>
        reads(string_view v) override
        {
            auto ret = to_number<long double>(v);
            if (ret.second == v)
                return nullopt;
            return ret;
        }
    };


    template <>
    struct ReadInstance<char> final : Read<char>
    {
        optional<std::pair<char,string_view>>
        reads(string_view v) override
        {
            auto v1 = cat::skipws(v);
            if (v1.empty())
                return nullopt;

            auto c = v1.front(); v1.remove_prefix(1);
            return std::make_pair(c, v1);
        }
    };

    template <>
    struct ReadInstance<unsigned char> final : Read<unsigned char>
    {
        optional<std::pair<unsigned char,string_view>>
        reads(string_view v) override
        {
            auto v1 = cat::skipws(v);
            if (v1.empty())
                return nullopt;

            auto c = v1.front(); v1.remove_prefix(1);
            return std::make_pair(static_cast<unsigned char>(c), v1);
        }
    };

    template <>
    struct ReadInstance<bool> final : Read<bool>
    {
        optional<std::pair<bool,string_view>>
        reads(string_view v) override
        {
            std::istringstream in(v.to_string());
            bool x; in >> std::noboolalpha;
            if (!(in >> x)) {
                in.clear();
                if (!(in >> std::boolalpha >> x))
                    return nullopt;
            }
            v.remove_prefix(in.tellg());
            return std::make_pair(x, v);
        }
    };

}
