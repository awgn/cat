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
#include <cat/bits/utility.hpp>

namespace cat
{
    //
    // class Monoid:
    //

    template <typename M>
    struct Monoid
    {
        template <template <typename ...> class F>
        struct Class
        {
            virtual M mempty() = 0;
            virtual M mappend(M const &, M const &) = 0;

            virtual M mconcat(F<M> const &xs)
            {
                auto r = mempty();
                for(auto const & x : xs)
                    r = mappend(r, x);
                return r;
            };
        };
    };

    template <typename M, template <typename ...> class> struct MonoidInstance;

    template <typename M>
    M mempty()
    {
        return MonoidInstance<M, std::vector>{}.mempty();
    }

    template <typename M>
    M mappend(M const &a, M const &b)
    {
        return MonoidInstance<M, std::vector>{}.mappend(a, b);
    }

    template <template <typename ...> class F, typename M>
    M mconcat(F<M> const &f)
    {
        return MonoidInstance<M, F>{}.mconcat(f);
    }

    template <typename M>
    struct is_monoid : std::false_type
    { };


} // namespace cat

