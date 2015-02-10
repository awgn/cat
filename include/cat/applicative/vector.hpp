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

#include <vector>
#include <cat/applicative/applicative.hpp>

namespace cat
{
    // vector is an applicative instance:
    //

    template <> struct is_applicative<std::vector> : std::true_type { };

    // vector instance:
    //

    template <typename Fun, typename A, typename ...Ts>
    struct ApplicativeInstance<std::vector, Fun, A, Ts...> : Applicative<std::vector>::Class<Fun, A, Ts...>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        std::vector<A>
        pure(A const &elem) final
        {
            return std::vector<A>{ elem };
        }

        std::vector<B>
        apply(std::vector<Fun> const &fs, std::vector<A, Ts...> const &xs) final
        {
            std::vector<B> out;
            out.reserve(fs.size() * xs.size());

            for(auto const &f : fs)
            {
                for(auto const &x : xs)
                {
                    out.push_back(f(x));
                }
            }

            return out;
        }
    };

} // namespace cat

