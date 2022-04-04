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
#include <string_view>

namespace cat
{
    //
    // Instances...
    //

    template <>
    struct ReadInstance<short> final : Read<short>
    {
        std::optional<std::pair<short,std::string_view>>
        reads(std::string_view v) override
        {
            auto ret = to_number<short>(v);
            if (ret.second == v)
                return std::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<int> final : Read<int>
    {
        std::optional<std::pair<int,std::string_view>>
        reads(std::string_view v) override
        {
            auto ret = to_number<int>(v);
            if (ret.second == v)
                return std::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<long int> final : Read<long int>
    {
        std::optional<std::pair<long int,std::string_view>>
        reads(std::string_view v) override
        {
            auto ret = to_number<long int>(v);
            if (ret.second == v)
                return std::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<long long int> final : Read<long long int>
    {
        std::optional<std::pair<long long int,std::string_view>>
        reads(std::string_view v) override
        {
            auto ret = to_number<long long int>(v);
            if (ret.second == v)
                return std::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<unsigned short> final : Read<unsigned short>
    {
        std::optional<std::pair<unsigned short,std::string_view>>
        reads(std::string_view v) override
        {
            auto ret = to_number<unsigned short>(v);
            if (ret.second == v)
                return std::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<unsigned> final : Read<unsigned>
    {
        std::optional<std::pair<unsigned,std::string_view>>
        reads(std::string_view v) override
        {
            auto ret = to_number<unsigned>(v);
            if (ret.second == v)
                return std::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<long unsigned> final : Read<long unsigned>
    {
        std::optional<std::pair<long unsigned,std::string_view>>
        reads(std::string_view v) override
        {
            auto ret = to_number<long unsigned>(v);
            if (ret.second == v)
                return std::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<long long unsigned> final : Read<long long unsigned>
    {
        std::optional<std::pair<long long unsigned,std::string_view>>
        reads(std::string_view v) override
        {
            auto ret = to_number<long long unsigned>(v);
            if (ret.second == v)
                return std::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<float> final : Read<float>
    {
        std::optional<std::pair<float,std::string_view>>
        reads(std::string_view v) override
        {
            auto ret = to_number<float>(v);
            if (ret.second == v)
                return std::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<double> final : Read<double>
    {
        std::optional<std::pair<double,std::string_view>>
        reads(std::string_view v) override
        {
            auto ret = to_number<double>(v);
            if (ret.second == v)
                return std::nullopt;
            return ret;
        }
    };

    template <>
    struct ReadInstance<long double> final : Read<long double>
    {
        std::optional<std::pair<long double,std::string_view>>
        reads(std::string_view v) override
        {
            auto ret = to_number<long double>(v);
            if (ret.second == v)
                return std::nullopt;
            return ret;
        }
    };


    template <>
    struct ReadInstance<char> final : Read<char>
    {
        std::optional<std::pair<char,std::string_view>>
        reads(std::string_view v) override
        {
            auto v1 = cat::skipws(v);
            if (v1.empty())
                return std::nullopt;

            auto c = v1.front(); v1.remove_prefix(1);
            return std::make_pair(c, v1);
        }
    };

    template <>
    struct ReadInstance<unsigned char> final : Read<unsigned char>
    {
        std::optional<std::pair<unsigned char,std::string_view>>
        reads(std::string_view v) override
        {
            auto v1 = cat::skipws(v);
            if (v1.empty())
                return std::nullopt;

            auto c = v1.front(); v1.remove_prefix(1);
            return std::make_pair(static_cast<unsigned char>(c), v1);
        }
    };

    template <>
    struct ReadInstance<bool> final : Read<bool>
    {
        std::optional<std::pair<bool,std::string_view>>
        reads(std::string_view v) override
        {
            std::istringstream in(std::string{v});
            bool x; in >> std::noboolalpha;
            if (!(in >> x)) {
                in.clear();
                if (!(in >> std::boolalpha >> x))
                    return std::nullopt;
            }
            v.remove_prefix(in.tellg());
            return std::make_pair(x, v);
        }
    };

}
