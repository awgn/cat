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

#include <cat/show/show.hpp>

#include <chrono>

namespace cat
{
    //
    // Instances...
    //

    namespace details
    {
        template <typename T> struct duration_traits;
        template <> struct duration_traits<std::chrono::nanoseconds>  { static constexpr const char *str = "ns"; };
        template <> struct duration_traits<std::chrono::microseconds> { static constexpr const char *str = "us"; };
        template <> struct duration_traits<std::chrono::milliseconds> { static constexpr const char *str = "ms"; };
        template <> struct duration_traits<std::chrono::seconds>      { static constexpr const char *str = "s"; };
        template <> struct duration_traits<std::chrono::minutes>      { static constexpr const char *str = "min"; };
        template <> struct duration_traits<std::chrono::hours>        { static constexpr const char *str = "h"; };
    }

    template <typename Rep, typename Period>
    struct ShowInstance<std::chrono::duration<Rep, Period>> final : Show<std::chrono::duration<Rep, Period>>
    {
        std::string
        show(std::chrono::duration<Rep, Period> const &dur)
        {
            std::string out(std::to_string(dur.count()));
            return std::move(out) + details::duration_traits<std::chrono::duration<Rep,Period>>::str;
        }
    };

    template <typename Clock, typename Dur>
    struct ShowInstance<std::chrono::time_point<Clock, Dur>> final : Show<std::chrono::time_point<Clock, Dur>>
    {
        std::string
        show(std::chrono::time_point<Clock, Dur> const &r)
        {
            return cat::show(r.time_since_epoch());
        }
    };

}
