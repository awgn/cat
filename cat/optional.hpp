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
#include <cat/bits/type.hpp>
#include <cat/container.hpp>
#include <cat/placeholders.hpp>

#include <experimental/optional>


namespace cat
{
    using std::experimental::optional;
    using std::experimental::make_optional;
    using std::experimental::nullopt_t;
    using std::experimental::nullopt;


    namespace details
    {
        using namespace placeholders;

        struct Maybe_
        {
            template <typename V, typename Fun, typename T>
            constexpr V operator()(V const & def, Fun fun, optional<T> const & value) const
            {
                return value ?  fun(value.value()) : def;
            }
        };

        struct CatOptionals_
        {
            using function_type =  _C<_a>(_C<optional<_a>>);

            template <template <typename...> class Cont, typename T>
            constexpr
            auto operator()(Cont<optional<T>> const &in) const
            {
                Cont<T> ret;

                for(auto & elem: in)
                    if (elem)
                        insert(ret, elem.value());

                return ret;
            }

            template <template <typename...> class Cont, typename T>
            constexpr
            auto operator()(Cont<optional<T>> && in) const
            {
                Cont<T> ret;

                for(auto & elem: in)
                    if (elem)
                        insert(ret, std::move(elem.value()));

                return ret;
            }
        };


        struct MapOptional_
        {
            using function_type =  _C<_b>(_f<optional<_b>(_a)>, _C<_a>);

            template <typename Fun, template <typename...> class Cont, typename T>
            constexpr auto operator()(Fun f, Cont<T> const &xs) const
            {
                Cont<typename decltype(f(std::declval<T>()))::value_type> ret;

                for(auto & x : xs)
                {
                    auto y = f(x);
                    if (y)
                        insert(ret,y.value());
                }

                return ret;
            }

            template <typename Fun, template <typename...> class Cont, typename T>
            constexpr auto operator()(Fun f, Cont<T> && xs) const
            {
                Cont<typename decltype(f(std::declval<T>()))::value_type> ret;

                for(auto & x : xs)
                {
                    auto y = f(std::move(x));
                    if (y)
                        insert(ret, y.value());
                }

                return ret;
            }
        };
    }

    constexpr auto maybe = details::Maybe_{};
    constexpr auto cat_optionals = details::CatOptionals_{};
    constexpr auto map_optional = details::MapOptional_{};

} // namespace cat
