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

#include <cat/type_traits.hpp>

#include <utility>

namespace cat
{
    namespace details
    {
        template <typename C, typename P, typename V>
        inline bool insert_check(C &, std::pair<P, V> const &value)
        {
            return value.second;
        }
        template <typename C, typename Iterator>
        inline bool insert_check(C const &cont, Iterator it)
        {
            return it != std::end(cont);
        }
    }

    //
    //  generic insert...
    //

    // enabled for std::vector, std::dequeue, std::list
    //

    template <typename C, typename V,
              typename std::enable_if<
                !has_key_type<C>::value &&
                !has_container_type<C>::value>::type * = nullptr>
    bool
    insert(C &cont, V &&value)
    {
        cont.push_back(std::forward<V>(value));
        return true;
    }

    // enabled for std::stack, std::queue, std::priority_queue
    //

    template <typename C, typename V,
              typename std::enable_if<!has_key_type<C>::value &&
                                       has_container_type<C>::value>::type * = nullptr>
    bool
    insert(C &cont, V &&value)
    {
        cont.push(std::forward<V>(value));
        return true;
    }

    // enabled for std::set, std::multiset, std::unordered_set,
    // std::unordered_multiset, std::map, std::multimap, std::unordered_map
    // std::unordered_multimap...
    //

    template <typename C, typename V,
             typename std::enable_if<has_key_type<C>::value>::type * = nullptr>
    bool
    insert(C &cont, V && value)
    {
        auto res = cont.insert(std::forward<V>(value));
        return details::insert_check(cont, res);
    }

    //
    // special case for forward_list...  This is O(n)!
    //

    template <typename T, typename V>
    bool
    insert(std::forward_list<T> &cont, V &&value)
    {
        auto before_end = cont.before_begin();
        for(auto &e : cont) {
            (void)e;
            ++before_end;
        }
        cont.insert_after(before_end, std::forward<V>(value));
        return true;
    }


} // namespace cat
