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

#include <cat/iterator.hpp>
#include <cat/functor/functor.hpp>

#include <forward_list>

namespace cat
{
    // forward_list is a functor:
    //

    template <> struct is_functor<std::forward_list> : std::true_type { };

    // forward_list instance:
    //

    template <typename A, typename Fun, typename Fa_>
    struct FunctorInstance<std::forward_list<A>, Fun, Fa_> final : Functor<std::forward_list>::
    template where<A, Fun, Fa_>
    {
        using B = std::result_of_t<Fun(A)>;

        std::forward_list<B>
        fmap(Fun f, Fa_ && xs) override
        {
            return std::forward_list<B>
            {
                map_iterator( forward_iterator<Fa_>(std::begin(xs)), f ),
                map_iterator( forward_iterator<Fa_>(std::end(xs)), f )
            };
        }
    };


} // namespace cat

