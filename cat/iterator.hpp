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

#include <cat/functional.hpp>

#include <iterator>

//
// similar to boost::transform_iterator
//

namespace cat
{
    template <typename Fun, typename Iter>
    struct _map_iterator
    {
        using value_type        = typename std::iterator_traits<Iter>::value_type;
        using difference_type   = typename std::iterator_traits<Iter>::difference_type;
        using pointer           = typename std::iterator_traits<Iter>::pointer;
        using reference         = decltype(std::declval<Fun>()(std::declval<value_type>()));
        using iterator_category = std::bidirectional_iterator_tag;

        _map_iterator(Iter it, Fun fun = {})
        : fun_(std::move(fun))
        , it_(std::move(it))
        { }


        _map_iterator &
        operator++()
        {
            ++it_;
            return *this;
        }

        _map_iterator
        operator++(int)
        {
            auto r(*this);
            ++(*this);
            return r;
        }

        reference
        operator*() const
        {
            return fun_(*it_);
        }

        bool operator==(const _map_iterator &other) const
        {
            return it_ == other.it_;
        }

        bool operator!=(const _map_iterator &other) const
        {
            return it_ != other.it_;
        }

        Fun fun_;
        Iter it_;
    };

    template <typename Iter, typename Fun = Identity>
    auto map_iterator(Iter it, Fun f = identity)
    {
        return _map_iterator<Fun, Iter>(it, f);
    }


} // namespace cat
