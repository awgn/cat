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
#include <cat/monad/optional.hpp>

#include <type_traits>

namespace cat
{
    //
    // Instances...
    //

    template <typename T>
    struct ReadInstance<optional<T>> final : Read<optional<T>>
    {
        using ret_type = optional<std::pair<optional<T>,string_view>>;

        ret_type
        reads(string_view s) override
        {
            if (auto nothing = ((mreturn.in<optional>(s)
                        >>= curry(consume_char)('('))
                        >>= curry(consume_char)(')'))
                        >>= [](string_view s) -> ret_type
                            {
                                return mreturn_(std::make_pair(optional<T>{}, s));
                            })
                return nothing;

            return (mreturn.in<optional>(s)
                        >>= curry(consume_char)('('))
                        >>= [](string_view s1) -> ret_type {
                            return cat::reads<T>(s1) >>= [](auto p) {
                                return consume_char(')', p.second)
                                        >>= [&](string_view s2) -> ret_type
                                        {
                                            return mreturn_(std::make_pair(make_optional(std::move(p.first)), s2));
                                        };
                                };
                            };
        }
    };

}
