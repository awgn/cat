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
        pretty_container(const T &v)
        {
            std::string out;
            for(auto const & e : v)
                out += cat::pretty(e) + ' ';
            return out;
        }
    }

    template <typename T>
    struct PrettyInstance<std::initializer_list<T>> final : Pretty<std::initializer_list<T>>
    {
        std::string pretty(std::initializer_list<T> const &xs) override
        {
            return type_name<std::initializer_list<T>>() + "{ " + details::pretty_container(xs) + "}";
        }
    };

    template <typename T, std::size_t N>
    struct PrettyInstance<std::array<T, N>> final : Pretty<std::array<T,N>>
    {
        std::string pretty(std::array<T,N> const &xs) override
        {
            return type_name<std::array<T,N>>() + "[ " + details::pretty_container(xs) + "]";
        }
    };

    template <typename T>
    struct PrettyInstance<std::vector<T>> final : Pretty<std::vector<T>>
    {
        std::string pretty(std::vector<T> const &xs) override
        {
            return type_name<std::vector<T>>() + "[ " + details::pretty_container(xs) + "]";
        }
    };

    template <typename T>
    struct PrettyInstance<std::list<T>> final : Pretty<std::list<T>>
    {
        std::string pretty(std::list<T> const &xs) override
        {
            return type_name<std::list<T>>() + "[ " + details::pretty_container(xs) + "]";
        }
    };

    template <typename T>
    struct PrettyInstance<std::forward_list<T>> final : Pretty<std::forward_list<T>>
    {
        std::string pretty(std::forward_list<T> const &xs) override
        {
            return type_name<std::forward_list<T>>() + "[ " + details::pretty_container(xs) + "]";
        }
    };

    template <typename T>
    struct PrettyInstance<std::deque<T>> final : Pretty<std::deque<T>>
    {
        std::string pretty(std::deque<T> const &xs) override
        {
            return type_name<std::deque<T>>() + "[ " + details::pretty_container(xs) + "]";
        }
    };

    template <typename T>
    struct PrettyInstance<std::set<T>> final : Pretty<std::set<T>>
    {
        std::string pretty(std::set<T> const &xs) override
        {
            return type_name<std::set<T>>() + "[ " + details::pretty_container(xs) + "]";
        }
    };

    template <typename T>
    struct PrettyInstance<std::unordered_set<T>> final : Pretty<std::unordered_set<T>>
    {
        std::string pretty(std::unordered_set<T> const &xs) override
        {
            return type_name<std::unordered_set<T>>() + "[ " + details::pretty_container(xs) + "]";
        }
    };

    template <typename T>
    struct PrettyInstance<std::multiset<T>> final : Pretty<std::multiset<T>>
    {
        std::string pretty(std::multiset<T> const &xs) override
        {
            return type_name<std::multiset<T>>() + "[ " + details::pretty_container(xs) + "]";
        }
    };

    template <typename T>
    struct PrettyInstance<std::unordered_multiset<T>> final : Pretty<std::unordered_multiset<T>>
    {
        std::string pretty(std::unordered_multiset<T> const &xs) override
        {
            return type_name<std::unordered_multiset<T>>() + "[ " + details::pretty_container(xs) + "]";
        }
    };

    template <typename T, typename K>
    struct PrettyInstance<std::map<K,T>> final : Pretty<std::map<K,T>>
    {
        std::string pretty(std::map<K,T> const &xs) override
        {
            return type_name<std::map<K,T>>() + "[ " + details::pretty_container(xs) + "]";
        }
    };

    template <typename T, typename K>
    struct PrettyInstance<std::unordered_map<K,T>> final : Pretty<std::unordered_map<K,T>>
    {
        std::string pretty(std::unordered_map<K,T> const &xs) override
        {
            return type_name<std::unordered_map<K,T>>() + "[ " + details::pretty_container(xs) + "]";
        }
    };

    template <typename T, typename K>
    struct PrettyInstance<std::multimap<K,T>> final : Pretty<std::multimap<K,T>>
    {
        std::string pretty(std::multimap<K,T> const &xs) override
        {
            return type_name<std::multimap<K,T>>() + "[ " + details::pretty_container(xs) + "]";
        }
    };

    template <typename T, typename K>
    struct PrettyInstance<std::unordered_multimap<K,T>> final : Pretty<std::unordered_multimap<K,T>>
    {
        std::string pretty(std::unordered_multimap<K,T> const &xs) override
        {
            return type_name<std::unordered_multimap<K,T>>() + "[ " + details::pretty_container(xs) + "]";
        }
    };

    template <typename Cont>
    struct PrettyInstance<cat::View<Cont>> final : Pretty<cat::View<Cont>>
    {
        std::string pretty(cat::View<Cont> const &xs) override
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
            return type_name<cat::View<Cont>>() + out.str();
        }
    };

}
