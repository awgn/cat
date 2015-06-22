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

#include <cat/__config.hpp>

#include <utility>
#include <functional>
#include <algorithm>

namespace cat
{
    //
    // this is a very *basic* view container class inspired
    // by string_view class.
    //

    template <typename Cont>
    struct View
    {
        using value_type = Cont;

        Cont to_container() const
        {
            if (pred)
            {
                Cont ret;
                std::copy_if(begin, end, std::inserter(ret, std::end(ret)), pred);
                return ret;
            }

            return Cont(begin, end);
        }

        typename Cont::const_iterator begin;
        typename Cont::const_iterator end;
        std::function<bool(typename Cont::value_type const &)> pred;
    };


    template <template <typename ...> class Cont,
              typename Iter,
              typename Pred = std::function<bool(typename Iter::value_type const &)>>
    auto inline view(Iter begin,
                     Iter end,
                     Pred pred = std::function<bool(typename Iter::value_type const &)>{})
    {
        using value_type = typename Iter::value_type;

        return View<Cont<value_type>>{ std::move(begin), std::move(end), std::move(pred) };
    }


} // namespace cat
