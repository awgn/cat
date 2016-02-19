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
#include <cat/type_traits.hpp>
#include <cat/utility.hpp>
#include <cat/placeholders.hpp>
#include <cat/bits/missing.hpp>

#include <utility>
#include <forward_list>
#include <initializer_list>
#include <iterator>

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
    //  generic inserter for different containers...
    //

    // enabled for std::vector, std::dequeue, std::list
    //

    template <typename C, typename T,
    std::enable_if_t<!has_key_type<C>::value &&
                     !has_container_type<C>::value> * = nullptr>
    bool insert(C &cont, T && x)
    {
        cont.push_back(std::forward<T>(x));
        return true;
    }

    template <typename C, typename It,
    std::enable_if_t<!has_key_type<C>::value &&
                     !has_container_type<C>::value> * = nullptr>
    bool insert(C &cont, It beg, It end)
    {
        cont.insert(std::end(cont), beg, end);
        return true;
    }


    // enabled for std::stack, std::queue, std::priority_queue
    //

    template <typename C, typename V,
    std::enable_if_t<!has_key_type<C>::value &&
                      has_container_type<C>::value> * = nullptr>
    bool insert(C &cont, V &&value)
    {
        cont.push(std::forward<V>(value));
        return true;
    }

    template <typename C, typename It,
    std::enable_if_t<!has_key_type<C>::value &&
                      has_container_type<C>::value> * = nullptr>
    bool insert(C &cont, It it, It end)
    {
        for(; it != end; ++it)
            cont.push(*it);
        return true;
    }


    // enabled for std::set, std::multiset, std::unordered_set,
    // std::unordered_multiset, std::map, std::multimap, std::unordered_map
    // std::unordered_multimap...
    //

    template <typename C, typename V,
    std::enable_if_t<has_key_type<C>::value> * = nullptr>
    bool
    insert(C &cont, V && value)
    {
        auto res = cont.insert(std::forward<V>(value));
        return details::insert_check(cont, res);
    }

    template <typename C, typename It,
    std::enable_if_t<has_key_type<C>::value> * = nullptr>
    bool insert(C &cont, It it, It end)
    {
        for(; it != end; ++it) {
            auto res = cont.insert(*it);
            if (!details::insert_check(cont, res))
                return false;
        }
        return true;
    }

    //
    // special case for forward_list...  This is O(n)!
    //

    template <typename T, typename V>
    bool insert(std::forward_list<T> &cont, V &&value)
    {
        auto before_end = cont.before_begin();
        for(auto &e : cont) {
            (void)e;
            ++before_end;
        }
        cont.insert_after(before_end, std::forward<V>(value));
        return true;
    }

    template <typename T, typename It>
    bool insert(std::forward_list<T> &cont, It it, It end)
    {
        auto before_end = cont.before_begin();
        for(auto &e : cont) {
            (void)e;
            ++before_end;
        }

        for(; it != end; ++it)
        {
            cont.insert_after(before_end, *it);
            ++before_end;
        }

        return true;
    }

    //
    // generic inserter for initializer_list
    //

    template <typename C, typename T>
    bool insert(C &cont, std::initializer_list<T> xs)
    {
        return insert(cont, std::begin(xs), std::end(xs));
    }

    //
    // generic fold operations for containers (overridden by Foldable)
    //

    namespace container
    {
        template <typename T,
                  std::enable_if_t<!is_pair<std::decay_t<T>>::value> * = nullptr>
        decltype(auto) iterator_elem(T &&value)
        {
            return std::forward<T>(value);
        }

        template <typename T,
                  std::enable_if_t<is_pair<std::decay_t<T>>::value> * = nullptr>
        decltype(auto) iterator_elem(T &&value)
        {
            return forward_as<T>(value.second);
        }

        //
        // foldl, foldl1, foldr, foldr1
        //

        using namespace placeholders;

        struct Foldl_
        {
            using function_type = _a(_f<_a(_a, _b)>, _a, _C<_b>);

            template <typename F, typename A, typename Cont>
            auto operator()(F f, A acc, Cont &&xs) const
            {
                for(auto it = std::begin(xs); it != std::end(xs); ++it)
                    acc = f(std::move(acc), iterator_elem(forward_as<Cont>(*it)));

                return acc;
            }
        };

        struct Foldl1_
        {
            using function_type = _a(_f<_a(_a, _a)>, _C<_a>);

            template <typename F, typename Cont>
            auto operator()(F f, Cont &&xs) const
            {
                if (xs.empty())
                    throw std::runtime_error("foldl1: empty container");

                auto acc = iterator_elem(forward_as<Cont>(*std::begin(xs)));

                for(auto it = std::next(std::begin(xs)); it != std::end(xs); ++it)
                    acc = f(std::move(acc), iterator_elem(forward_as<Cont>(*it)));

                return acc;
            }
        };

        struct Foldr_
        {
            using function_type = _b(_f<_b(_a, _b)>, _b, _C<_a>);

            template <typename F, typename A, typename Cont>
            auto operator()(F f, A acc, Cont &&xs) const
            {
                for(auto it = xs.rbegin(); it != xs.rend(); ++it)
                    acc = f(iterator_elem(forward_as<Cont>(*it)), std::move(acc));

                return acc;
            }
        };

        struct Foldr1_
        {
            using function_type = _a(_f<_a(_a, _a)>, _C<_a>);

            template <typename F, typename Cont>
            auto operator()(F f, Cont &&xs) const
            {
                if (xs.empty())
                    throw std::runtime_error("foldr1: empty container");

                auto acc = iterator_elem(forward_as<Cont>(*xs.rbegin()));

                for(auto it = std::next(xs.rbegin()); it != xs.rend(); ++it)
                    acc = f(iterator_elem(forward_as<Cont>(*it)), std::move(acc));

                return acc;
            }
        };

        constexpr auto foldl  = container::Foldl_{};
        constexpr auto foldl1 = container::Foldl1_{};
        constexpr auto foldr  = container::Foldr_{};
        constexpr auto foldr1 = container::Foldr1_{ };
    }

} // namespace cat
