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

#include <cat/pretty/pretty.hpp>
#include <cat/show/show.hpp>
#include <cat/show/chrono.hpp>

#include <chrono>

namespace cat
{
    //
    // Instances...
    //

    template <typename Rep, typename Period>
    struct PrettyInstance<std::chrono::duration<Rep, Period>> final : Pretty<std::chrono::duration<Rep, Period>>
    {
        std::string
        pretty(std::chrono::duration<Rep, Period> const &dur)
        {
            std::string out(std::to_string(dur.count()));
            return type_name<std::chrono::duration<Rep, Period>>() + '{' + std::move(out) + details::duration_traits<std::chrono::duration<Rep,Period>>::str + '}';
        }
    };

    template <typename Clock, typename Dur>
    struct PrettyInstance<std::chrono::time_point<Clock, Dur>> final : Pretty<std::chrono::time_point<Clock, Dur>>
    {
        std::string
        pretty(std::chrono::time_point<Clock, Dur> const &r)
        {
            return cat::pretty(r.time_since_epoch());
        }
    };

}
