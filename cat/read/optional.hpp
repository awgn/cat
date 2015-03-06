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
        optional<std::pair<optional<T>,string_view>>
        reads(string_view v) override
        {
            return consume('(', v) >>= [](string_view s1)
                -> optional<std::pair< optional<T>, string_view>>
            {
                if (auto val = cat::reads<T>(s1) >>= [&] (auto const &t) {
                        return consume(')', t.second) >>= [&](string_view left) {
                            return mreturn.in<optional>(std::make_pair(make_optional(t.first), left));
                        }; })
                    return val;

                if (auto nothing = consume(')', s1))
                    return mreturn.in<optional>(std::make_pair(optional<T>{}, nothing.value()));

                return nullopt;
            };
        }
    };

}
