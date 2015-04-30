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

#include <cat/read.hpp>
#include <cat/show.hpp>
#include <cat/functor.hpp>
#include <cat/monad.hpp>
#include <cat/functional.hpp>
#include <cat/type_traits.hpp>
#include <cat/string_view.hpp>

#include <cat/bits/type.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <stdexcept>


namespace parser
{
    template <typename T>
    struct key_name
    {
        static std::string name() { return cat::type_name<T>(); }
    };

    #define DECLARE_KEY(k) struct k : parser::key_name<k> { }

    //
    // key_value
    //

    template <typename Key, typename Value>
    struct key_value
    {
        static_assert(cat::is_showable<Value>::value, "key_value: Value is not showable!");
        static_assert(cat::is_readable<Value>::value, "key_value: Value is not readable!");

        typedef Key     key_type;
        typedef Value   value_type;

        Value   value;
    };

    template <typename Key, typename Value>
    auto
    make_key_value(Value && v)
    {
        return key_value<Key, std::remove_reference_t<Value>> { std::forward<Value>(v) };
    }

    namespace details
    {
        template <typename K, typename ...Ts> struct key_index;
        template <typename K, typename V, typename ...Ts>
        struct key_index<K, key_value<K, V>, Ts...>
        {
            enum { value = 0 };
        };
        template <typename K, typename T0, typename ...Ts>
        struct key_index<K, T0, Ts...>
        {
            enum { value = 1 + key_index<K, Ts...>::value};
        };
    }

    //
    // document
    //

    template <typename ...KVs>
    struct document
    {
        using type = std::tuple<KVs...>;

        template <typename K>
        auto &
        get() const
        {
            return std::get<details::key_index<K, KVs...>::value >(tuple_).value;
        }

        template <typename K>
        auto &
        get()
        {
            return std::get<details::key_index<K, KVs...>::value >(tuple_).value;
        }

        std::tuple<KVs...> tuple_;
    };

    //
    // read string from file...
    //

    inline std::string
    read_file(const char *filename)
    {
        std::ifstream ifs(filename);
        if (!ifs)
            throw std::runtime_error(std::string(filename) + ": no such file");

        return std::string(std::istreambuf_iterator<char>(ifs.rdbuf()),
                           std::istreambuf_iterator<char>());
    }

    //
    // remove comments from document string
    //

    inline std::string
    remove_comments(std::string const & data, char commkey)
    {
        struct par_state
        {
            enum class state { none, string1, string2, comment, backslash };

            par_state(char ckey)
            : state_{state::none}
            , prev_{state::none}
            , comm_ {ckey}
            { }

            char operator()(char c)
            {
                auto prev = state_;
                state_ = next(c);
                prev_ = prev;

                return state_ == state::comment ? ' ' : c;
            }

            state next(char c)
            {
                switch(state_)
                {
                case state::none:
                    return
                        c == '\''   ? state::string1   :
                        c == '"'    ? state::string2   :
                        c == comm_  ? state::comment   :
                        c == '\\'   ? state::backslash : state::none;

                case state::string1:
                    return
                        c == '\''   ? state::none      :
                        c == '\\'   ? state::backslash : state::string1;

                case state::string2:
                    return
                        c == '"'    ? state::none      :
                        c == '\\'   ? state::backslash : state::string2;

                case state::comment:
                    return
                        c == '\n'   ? state::none      : state::comment;

                case state::backslash:
                    return prev_;
                }

                return state::none;
            }

            state state_;
            state prev_;
            char  comm_;

        } parser(commkey);

        return cat::fmap(parser, data);
    }
}


namespace cat
{
    //
    // key_value instances...
    //

    template <typename Key, typename Value>
    struct ShowInstance<parser::key_value<Key,Value>> final : Show<parser::key_value<Key,Value>>
    {
        static_assert(is_showable<Value>::value, "key_value: Value is not showable!");

        std::string
        show(parser::key_value<Key, Value> const &v) override
        {
            return cat::type_name<Key>() + " = " + cat::show(v.value);
        }
    };

    template <typename K, typename V>
    struct ReadInstance<parser::key_value<K,V>> final : Read<parser::key_value<K,V>>
    {
        using ret_type = optional<std::pair<parser::key_value<K,V>, string_view>>;

        ret_type
        reads(string_view s) override
        {
            return ((cat::consume_string(K::name().c_str(), s)
                    >>= curry(cat::consume_char) ('='))
                    >>= cat::reads<V>)
                    >>= [](auto val) -> ret_type
                        {
                            return mreturn_(std::make_pair(parser::make_key_value<K>(std::move(val.first)), std::move(val.second)));
                        };
        }
    };

    //
    // document instances...
    //

    template <typename ...KVs>
    struct ShowInstance<parser::document<KVs...>> final : Show<parser::document<KVs...>>
    {
        std::string
        show(parser::document<KVs...> const &doc) override
        {
            return "{ " + cat::show(doc.tuple_) + "}";
        }
    };


    template <typename ...KVs>
    struct ReadInstance<parser::document<KVs...>> final : Read<parser::document<KVs...>>
    {
        optional<std::pair<parser::document<KVs...>, string_view>>
        reads(string_view const s) override
        {
            parser::document<KVs...> doc;

            auto body = [&](string_view s1) -> optional<string_view>
                        {
                            while (s1 = cat::skipws(s1), !s1.empty())
                            {
                                auto s2 = cat::read_any(doc.tuple_, s1);
                                if (!s2)
                                    break;
                                s1 = std::move(s2.value().second);
                            }

                            return mreturn_(s1);
                        };

            auto p1 = ((mreturn.in<optional>(s) >>= curry(cat::consume_char)('{')) >>= body) >>= curry(cat::consume_char)('}');
            auto p2 =   mreturn.in<optional>(s) >>= body;

            if (p1)
                return make_optional(std::make_pair(std::move(doc), p1.value()));

            if (cat::skipws(p2.value()).empty())
                return make_optional(std::make_pair(std::move(doc), string_view{}));

            throw std::runtime_error("document: error@" + std::string(p2.value().substr(0,p2.value().find('\n'))));
        }
    };
}

