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
#include <cat/string_view.hpp>
#include <cat/optional.hpp>

#include <string>
#include <cstring>
#include <type_traits>

namespace cat
{
    //
    // Instances...
    //

    template <>
    struct ReadInstance<std::string> final : Read<std::string>
    {
        optional<std::pair<std::string, string_view>>
        reads(string_view s) override
        {
            std::string str;

            bool quoted = false, escaped = false;

            s = skipws(s);
            if (auto s_ = consume_char('"', s))
            {
                s = std::move(s_.value());
                quoted = true;
            }

            for(;!s.empty(); s.remove_prefix(1))
            {
                auto c = s.front();

                if (escaped) {
                    escaped = false;
                    str.push_back(c);
                    continue;
                }

                if (quoted && c == '"')
                {
                    s.remove_prefix(1);
                    return std::make_pair(std::move(str), s);
                }

                if (!quoted && std::isspace(c))
                {
                    return std::make_pair(std::move(str), s);
                }

                if (c == '\\')
                    escaped = true;
                else
                    str.push_back(c);
            }

            // end-of-string...
            //
            if (quoted || str.empty())
                    return nullopt;

            return std::make_pair(std::move(str), s);
        }
    };

}
