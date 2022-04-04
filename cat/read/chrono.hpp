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
#include <cat/optional.hpp>
#include <cat/monad/optional.hpp>

#include <chrono>
#include <string_view>

namespace cat
{
    //
    // Instances...
    //

    template <typename Rep, typename Period>
    struct ReadInstance<std::chrono::duration<Rep, Period>> final : Read<std::chrono::duration<Rep, Period>>
    {

        std::optional<std::pair<std::chrono::duration<Rep, Period>, std::string_view>>
        reads(std::string_view s) override
        {
            using Duration = std::chrono::duration<Rep, Period>;

            return cat::reads<int64_t>(s) >>= [&] (auto const &value) {
                return cat::consume_char('_', value.second) >>= [&] (auto const &l2) {
                    return cat::reads<std::string>(l2) >>= [&] (auto const &unit)
                        -> std::optional<std::pair<std::chrono::duration<Rep, Period>, std::string_view>>
                    {
                        if (unit.first.compare("ns") == 0)
                            return std::make_pair(std::chrono::duration_cast<Duration>(std::chrono::nanoseconds(value.first)), unit.second);
                        if (unit.first.compare("us") == 0)
                            return std::make_pair(std::chrono::duration_cast<Duration>(std::chrono::microseconds(value.first)), unit.second);
                        if (unit.first.compare("ms") == 0)
                            return std::make_pair(std::chrono::duration_cast<Duration>(std::chrono::milliseconds(value.first)), unit.second);
                        if (unit.first.compare("s") == 0)
                            return std::make_pair(std::chrono::duration_cast<Duration>(std::chrono::seconds(value.first)), unit.second);
                        if (unit.first.compare("min") == 0)
                            return std::make_pair(std::chrono::duration_cast<Duration>(std::chrono::minutes(value.first)), unit.second);
                        if (unit.first.compare("h") == 0)
                            return std::make_pair(std::chrono::duration_cast<Duration>(std::chrono::hours(value.first)), unit.second);

                        return std::nullopt;
                    };
                };
            };
        }
    };

    template <typename Clock, typename Dur>
    struct ReadInstance<std::chrono::time_point<Clock, Dur>> final : Read<std::chrono::time_point<Clock, Dur>>
    {
        std::optional<std::pair<std::chrono::time_point<Clock, Dur>, std::string_view>>
        reads(std::string_view s) override
        {
            if (auto d = cat::reads<Dur>(s))
            {
                return std::make_pair(std::chrono::time_point<Clock, Dur>((*d).first), (*d).second);
            }
            return std::nullopt;
        }
    };
}
