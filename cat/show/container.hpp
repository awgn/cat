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
#include <cat/type_traits.hpp>
#include <cat/view.hpp>

#include <initializer_list>
#include <sstream>
#include <array>
#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

namespace cat
{
    //
    // Instances...
    //

    namespace details
    {
        template <typename T>
        std::string
        show_container(const T &v)
        {
            std::string out;
            for(auto const & e : v)
                out += cat::show(e) + ' ';
            return out;
        }
    }

    template <typename T>
    struct ShowInstance<std::initializer_list<T>> final : Show<std::initializer_list<T>>
    {
        std::string show(std::initializer_list<T> const &xs) override
        {
            return "{ " + details::show_container(xs) + "}";
        }
    };

    template <typename T, std::size_t N>
    struct ShowInstance<std::array<T, N>> final : Show<std::array<T,N>>
    {
        std::string show(std::array<T,N> const &xs) override
        {
            return "[ " + details::show_container(xs) + "]";
        }
    };

    template <typename T>
    struct ShowInstance<std::vector<T>> final : Show<std::vector<T>>
    {
        std::string show(std::vector<T> const &xs) override
        {
            return "[ " + details::show_container(xs) + "]";
        }
    };

    template <typename T>
    struct ShowInstance<std::list<T>> final : Show<std::list<T>>
    {
        std::string show(std::list<T> const &xs) override
        {
            return "[ " + details::show_container(xs) + "]";
        }
    };

    template <typename T>
    struct ShowInstance<std::forward_list<T>> final : Show<std::forward_list<T>>
    {
        std::string show(std::forward_list<T> const &xs) override
        {
            return "[ " + details::show_container(xs) + "]";
        }
    };

    template <typename T>
    struct ShowInstance<std::deque<T>> final : Show<std::deque<T>>
    {
        std::string show(std::deque<T> const &xs) override
        {
            return "[ " + details::show_container(xs) + "]";
        }
    };

    template <typename T>
    struct ShowInstance<std::set<T>> final : Show<std::set<T>>
    {
        std::string show(std::set<T> const &xs) override
        {
            return "[ " + details::show_container(xs) + "]";
        }
    };

    template <typename T>
    struct ShowInstance<std::unordered_set<T>> final : Show<std::unordered_set<T>>
    {
        std::string show(std::unordered_set<T> const &xs) override
        {
            return "[ " + details::show_container(xs) + "]";
        }
    };

    template <typename T>
    struct ShowInstance<std::multiset<T>> final : Show<std::multiset<T>>
    {
        std::string show(std::multiset<T> const &xs) override
        {
            return "[ " + details::show_container(xs) + "]";
        }
    };

    template <typename T>
    struct ShowInstance<std::unordered_multiset<T>> final : Show<std::unordered_multiset<T>>
    {
        std::string show(std::unordered_multiset<T> const &xs) override
        {
            return "[ " + details::show_container(xs) + "]";
        }
    };

    template <typename T, typename K>
    struct ShowInstance<std::map<K,T>> final : Show<std::map<K,T>>
    {
        std::string show(std::map<K,T> const &xs) override
        {
            return "[ " + details::show_container(xs) + "]";
        }
    };

    template <typename T, typename K>
    struct ShowInstance<std::unordered_map<K,T>> final : Show<std::unordered_map<K,T>>
    {
        std::string show(std::unordered_map<K,T> const &xs) override
        {
            return "[ " + details::show_container(xs) + "]";
        }
    };

    template <typename T, typename K>
    struct ShowInstance<std::multimap<K,T>> final : Show<std::multimap<K,T>>
    {
        std::string show(std::multimap<K,T> const &xs) override
        {
            return "[ " + details::show_container(xs) + "]";
        }
    };

    template <typename T, typename K>
    struct ShowInstance<std::unordered_multimap<K,T>> final : Show<std::unordered_multimap<K,T>>
    {
        std::string show(std::unordered_multimap<K,T> const &xs) override
        {
            return "[ " + details::show_container(xs) + "]";
        }
    };

    template <typename Cont>
    struct ShowInstance<cat::View<Cont>> final : Show<cat::View<Cont>>
    {
        std::string show(cat::View<Cont> const &xs) override
        {
            std::ostringstream out;
            out << "[ ";

            auto it = xs.begin, it_e = xs.end;
            for(; it != it_e; ++it)
            {
                if (!xs.pred || (xs.pred && xs.pred(*it)))
                        out << *it << ' ';
            }

            out << "]";
            return out.str();
        }
    };

}
