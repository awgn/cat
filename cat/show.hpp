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

#include <cat/bits/type.hpp>
#include <cat/type_traits.hpp>
#include <cat/tuple.hpp>

#include <initializer_list>
#include <sstream>
#include <string>
#include <memory>
#include <array>
#include <chrono>

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
    // instance
    //

    template <typename T> struct ShowInstance
    {
        using non_specialized = int;
    };

    //
    // trait for concepts
    //

    template <typename T>
    struct is_showable : has_specialization<ShowInstance, T>
    { };

    //
    // free function
    //

    template <typename T>
    std::string show(T const &v)
    {
        static_assert(is_showable<T>::value, "T is not showable!");
        return ShowInstance<T>{}.show(v);
    }

    //
    // type class Show
    //

    template <typename T>
    struct Show
    {
        virtual std::string show(T const &) = 0;
    };

    //
    // Instances...
    //

    template <>
    struct ShowInstance<short> final : Show<short>
    {
        std::string show(short const &v) { return std::to_string(v); }
    };

    template <>
    struct ShowInstance<int> final : Show<int>
    {
        std::string show(int const &v) { return std::to_string(v); }
    };
    template <>
    struct ShowInstance<long int> final : Show<long int>
    {
        std::string show(long int const &v) { return std::to_string(v); }
    };
    template <>
    struct ShowInstance<long long int> final : Show<long long int>
    {
        std::string show(long long int const &v) { return std::to_string(v); }
    };

    template <>
    struct ShowInstance<short unsigned> final : Show<short unsigned>
    {
        std::string show(short unsigned const &v) { return std::to_string(v); }
    };
    template <>
    struct ShowInstance<unsigned> final : Show<unsigned>
    {
        std::string show(unsigned const &v) { return std::to_string(v); }
    };
    template <>
    struct ShowInstance<long unsigned> final : Show<long unsigned>
    {
        std::string show(long unsigned const &v) { return std::to_string(v); }
    };
    template <>
    struct ShowInstance<long long unsigned> final : Show<long long unsigned>
    {
        std::string show(long long unsigned const &v) { return std::to_string(v); }
    };

    template <>
    struct ShowInstance<float> final : Show<float>
    {
        std::string show(const float &v) { return std::to_string(v); }
    };
    template <>
    struct ShowInstance<double> final : Show<double>
    {
        std::string show(const double &v) { return std::to_string(v); }
    };
    template <>
    struct ShowInstance<long double> final : Show<long double>
    {
        std::string show(const long double &v) { return std::to_string(v); }
    };


    template <>
    struct ShowInstance<std::string> final : Show<std::string>
    {
        std::string show(std::string const &v)
        {
            return '"' + v + '"';
        }
    };

    template <>
    struct ShowInstance<char> final : Show<char>
    {
        std::string show(char const &c) { return "'" + std::string(1, c) + "'"; }
    };

    template <>
    struct ShowInstance<bool> final : Show<bool>
    {
        std::string show(bool const &v) { return v ? "true" : "false"; }
    };

    template <>
    struct ShowInstance<const char *> final : Show<const char *>
    {
        std::string show(const char * const & s)
        {
            if (s)
                return '"' + std::string(s) + '"';
            return "nullptr";
        }
    };

    template <typename T>
    struct ShowInstance<T *> final : Show<T *>
    {
        std::string show(T * const &p)
        {
            std::ostringstream out;
            out << static_cast<const void *>(p);
            return out.str();
        }
    };

    template <typename T>
    struct ShowInstance<std::unique_ptr<T>> final : Show<std::unique_ptr<T>>
    {
        std::string show(std::unique_ptr<T> const &p)
        {
            std::ostringstream out;
            out << static_cast<const void *>(p.get()) << "_up";
            return out.str();
        }
    };

    template <typename T>
    struct ShowInstance<std::shared_ptr<T>> final : Show<std::shared_ptr<T>>
    {
        std::string show(std::shared_ptr<T> const &p)
        {
            std::ostringstream out;
            out << static_cast<const void *>(p.get()) << "_sp" << p.use_count();
            return out.str();
        }
    };

    template <typename T, typename V>
    struct ShowInstance<std::pair<T, V>> final : Show<std::pair<T,V>>
    {
        std::string
        show(const std::pair<T,V> &p)
        {
            return  '(' + cat::show(p.first) + ' ' + cat::show(p.second) + ')';
        }
    };


    template <typename ...Ts>
    struct ShowInstance<std::tuple<Ts...>> final : Show<std::tuple<Ts...>>
    {
        std::string
        show(const std::tuple<Ts...> &t)
        {
            std::string out("(");

            tuple_foreach([&](auto const &elem) {
                                out += ' ' + cat::show(elem);
                          }, t);

            return std::move(out) + " )";
        }
    };

    namespace details
    {
        template <typename T> struct duration_traits;
        template <> struct duration_traits<std::chrono::nanoseconds>  { static constexpr const char *str = "_ns"; };
        template <> struct duration_traits<std::chrono::microseconds> { static constexpr const char *str = "_us"; };
        template <> struct duration_traits<std::chrono::milliseconds> { static constexpr const char *str = "_ms"; };
        template <> struct duration_traits<std::chrono::seconds>      { static constexpr const char *str = "_s"; };
        template <> struct duration_traits<std::chrono::minutes>      { static constexpr const char *str = "_m"; };
        template <> struct duration_traits<std::chrono::hours>        { static constexpr const char *str = "_h"; };
    }

    template <typename Rep, typename Period>
    struct ShowInstance<std::chrono::duration<Rep, Period>> final : Show<std::chrono::duration<Rep, Period>>
    {
        std::string
        show(std::chrono::duration<Rep, Period> const &dur)
        {
            std::string out(std::to_string(dur.count()));
            return std::move(out) + details::duration_traits<std::chrono::duration<Rep,Period>>::str;
        }
    };

    template <typename Clock, typename Dur>
    struct ShowInstance<std::chrono::time_point<Clock, Dur>> final : Show<std::chrono::time_point<Clock, Dur>>
    {
        std::string
        show(std::chrono::time_point<Clock, Dur> const &r)
        {
            return cat::show(r.time_since_epoch());
        }
    };

    template <typename T, T value>
    struct ShowInstance<std::integral_constant<T, value>> final : Show<std::integral_constant<T, value>>
    {
        std::string
        show(std::integral_constant<T, value> const &)
        {
            return cat::show(value) + "_" + type_name<T>();
        }
    };

    namespace details
    {
        template <typename T>
        std::string
        show_container(const T &v)
        {
            std::string out;
            for(auto const & e : v)
                out += cat::show(e) + ' ';
            return std::move(out);
        }
    }

    template <typename T>
    struct ShowInstance<std::initializer_list<T>> final : Show<std::initializer_list<T>>
    {
        std::string show(std::initializer_list<T> const &xs) { return "{ " + details::show_container(xs) + "}"; }
    };

    template <typename T, std::size_t N>
    struct ShowInstance<std::array<T, N>> final : Show<std::array<T,N>>
    {
        std::string show(std::array<T,N> const &xs) { return "[ " + details::show_container(xs) + "]"; }
    };

    template <typename T>
    struct ShowInstance<std::vector<T>> final : Show<std::vector<T>>
    {
        std::string show(std::vector<T> const &xs) { return "[ " + details::show_container(xs) + "]"; }
    };

    template <typename T>
    struct ShowInstance<std::list<T>> final : Show<std::list<T>>
    {
        std::string show(std::list<T> const &xs) { return "[ " + details::show_container(xs) + "]"; }
    };

    template <typename T>
    struct ShowInstance<std::forward_list<T>> final : Show<std::forward_list<T>>
    {
        std::string show(std::forward_list<T> const &xs) { return "[ " + details::show_container(xs) + "]"; }
    };

    template <typename T>
    struct ShowInstance<std::deque<T>> final : Show<std::deque<T>>
    {
        std::string show(std::deque<T> const &xs) { return "[ " + details::show_container(xs) + "]"; }
    };

    template <typename T>
    struct ShowInstance<std::set<T>> final : Show<std::set<T>>
    {
        std::string show(std::set<T> const &xs) { return "[ " + details::show_container(xs) + "]"; }
    };

    template <typename T>
    struct ShowInstance<std::unordered_set<T>> final : Show<std::unordered_set<T>>
    {
        std::string show(std::unordered_set<T> const &xs) { return "[ " + details::show_container(xs) + "]"; }
    };

    template <typename T>
    struct ShowInstance<std::multiset<T>> final : Show<std::multiset<T>>
    {
        std::string show(std::multiset<T> const &xs) { return "[ " + details::show_container(xs) + "]"; }
    };

    template <typename T>
    struct ShowInstance<std::unordered_multiset<T>> final : Show<std::unordered_multiset<T>>
    {
        std::string show(std::unordered_multiset<T> const &xs) { return "[ " + details::show_container(xs) + "]"; }
    };

    template <typename T, typename K>
    struct ShowInstance<std::map<T,K>> final : Show<std::map<T,K>>
    {
        std::string show(std::map<T,K> const &xs) { return "[ " + details::show_container(xs) + "]"; }
    };

    template <typename T, typename K>
    struct ShowInstance<std::unordered_map<T,K>> final : Show<std::unordered_map<T,K>>
    {
        std::string show(std::unordered_map<T,K> const &xs) { return "[ " + details::show_container(xs) + "]"; }
    };

    template <typename T, typename K>
    struct ShowInstance<std::multimap<T,K>> final : Show<std::multimap<T,K>>
    {
        std::string show(std::multimap<T,K> const &xs) { return "[ " + details::show_container(xs) + "]"; }
    };

    template <typename T, typename K>
    struct ShowInstance<std::unordered_multimap<T,K>> final : Show<std::unordered_multimap<T,K>>
    {
        std::string show(std::unordered_multimap<T,K> const &xs) { return "[ " + details::show_container(xs) + "]"; }
    };
}
