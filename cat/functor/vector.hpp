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

#include <cat/functor/functor.hpp>
#include <cat/utility.hpp>

#include <vector>

namespace cat
{
    // vector is a functor:
    //

    template <> struct is_functor<std::vector> : std::true_type { };

    // vector instance:
    //

    template <typename A, typename Fun, typename Fa_>
    struct FunctorInstance<std::vector<A>, Fun, Fa_> final : Functor<std::vector>::
    template where<A, Fun, Fa_>
    {
        using B = std::invoke_result_t<Fun, A>;

        std::vector<B>
        fmap(Fun f, Fa_ && xs) override
        {
            std::vector<B> out;
            out.reserve(xs.size());

            for(auto & x : xs)
                out.push_back(f(forward_as<Fa_>(x)));

            return out;
        }
    };

} // namespace cat
