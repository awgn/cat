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

#include <cat/read/read.hpp>
#include <cat/monad/optional.hpp>
#include <cat/container.hpp>        // generic inserter
#include <cat/type_traits.hpp>

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
        struct generic_decay
        {
            using type = std::decay_t<T>;
        };
        template <typename T, typename V>
        struct generic_decay<std::pair<T, V>>
        {
            using type = std::pair<std::decay_t<T>, std::decay_t<V>>;
        };
        template <typename ...Ts>
        struct generic_decay<std::tuple<Ts...>>
        {
            using type = std::tuple<std::decay_t<Ts>...>;
        };

        template <typename T>
        using generic_decay_t =typename generic_decay<T>::type;


        template <typename C>
        optional<std::pair<C,string_view>>
        read_container(string_view s)
        {
            if (auto s_ = consume_char('[', s))
            {
                s = std::move(s_.value());

                C ret;

                while(!s.empty())
                {
                    if (auto left = consume_char(']', s))
                        return make_optional(std::make_pair(ret, std::move(left.value())));

                    if (auto elem_ = cat::reads<generic_decay_t<typename C::value_type>>(s))
                    {
                        auto & elem = elem_.value();
                        s = std::move(elem.second);
                        cat::insert(ret, std::move(elem.first));
                    }
                    else
                        break;
                }
            }

            return nullopt;
        }
    }

    template <typename T, size_t N>
    struct ReadInstance<std::array<T, N>> final : Read<std::array<T,N>>
    {
        optional<std::pair<std::array<T,N>,string_view>>
        reads(string_view s) override
        {
            if (auto s_ = cat::consume_char('[', s))
            {
                s = std::move(s_.value());
                std::array<T, N> ret;

                for(auto & e : ret)
                {
                    if(auto elem_ = cat::reads<T>(s))
                    {
                        auto & elem = elem_.value();
                        s = elem.second;
                        e = std::move(elem.first);
                    }
                    else
                        break;
                }

                if (auto x_ = cat::consume_char(']', s))
                {
                    return std::make_pair(std::move(ret), std::move(x_.value()));
                }
            }

            return nullopt;
        }
    };

    template <typename T>
    struct ReadInstance<std::vector<T>> final : Read<std::vector<T>>
    {
        optional<std::pair<std::vector<T>,string_view>>
        reads(string_view s) override
        {
            return details::read_container<std::vector<T>>(s);
        }
    };

    template <typename T>
    struct ReadInstance<std::list<T>> final : Read<std::list<T>>
    {
        optional<std::pair<std::list<T>,string_view>>
        reads(string_view s) override
        {
            return details::read_container<std::list<T>>(s);
        }
    };

    template <typename T>
    struct ReadInstance<std::forward_list<T>> final : Read<std::forward_list<T>>
    {
        optional<std::pair<std::forward_list<T>,string_view>>
        reads(string_view s) override
        {
            return details::read_container<std::forward_list<T>>(s);
        }
    };

    template <typename T>
    struct ReadInstance<std::deque<T>> final : Read<std::deque<T>>
    {
        optional<std::pair<std::deque<T>,string_view>>
        reads(string_view s) override
        {
            return details::read_container<std::deque<T>>(s);
        }
    };

    template <typename T>
    struct ReadInstance<std::set<T>> final : Read<std::set<T>>
    {
        optional<std::pair<std::set<T>,string_view>>
        reads(string_view s) override
        {
            return details::read_container<std::set<T>>(s);
        }
    };

    template <typename T>
    struct ReadInstance<std::multiset<T>> final : Read<std::multiset<T>>
    {
        optional<std::pair<std::multiset<T>,string_view>>
        reads(string_view s) override
        {
            return details::read_container<std::multiset<T>>(s);
        }
    };

    template <typename T>
    struct ReadInstance<std::unordered_set<T>> final : Read<std::unordered_set<T>>
    {
        optional<std::pair<std::unordered_set<T>,string_view>>
        reads(string_view s) override
        {
            return details::read_container<std::unordered_set<T>>(s);
        }
    };

    template <typename T>
    struct ReadInstance<std::unordered_multiset<T>> final : Read<std::unordered_multiset<T>>
    {
        optional<std::pair<std::unordered_multiset<T>,string_view>>
        reads(string_view s) override
        {
            return details::read_container<std::unordered_multiset<T>>(s);
        }
    };

    template <typename K, typename T>
    struct ReadInstance<std::map<K, T>> final : Read<std::map<K, T>>
    {
        optional<std::pair<std::map<K, T>,string_view>>
        reads(string_view s) override
        {
            return details::read_container<std::map<K, T>>(s);
        }
    };

    template <typename K, typename T>
    struct ReadInstance<std::multimap<K, T>> final : Read<std::multimap<K, T>>
    {
        optional<std::pair<std::multimap<K, T>,string_view>>
        reads(string_view s) override
        {
            return details::read_container<std::multimap<K, T>>(s);
        }
    };

    template <typename K, typename T>
    struct ReadInstance<std::unordered_map<K, T>> final : Read<std::unordered_map<K, T>>
    {
        optional<std::pair<std::unordered_map<K, T>,string_view>>
        reads(string_view s) override
        {
            return details::read_container<std::unordered_map<K, T>>(s);
        }
    };

    template <typename K, typename T>
    struct ReadInstance<std::unordered_multimap<K, T>> final : Read<std::unordered_multimap<K, T>>
    {
        optional<std::pair<std::unordered_multimap<K, T>,string_view>>
        reads(string_view s) override
        {
            return details::read_container<std::unordered_multimap<K, T>>(s);
        }
    };

}
