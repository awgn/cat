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

#include <cat/placeholders.hpp>
#include <cat/type_traits.hpp>
#include <cat/utility.hpp>

#include <vector>

namespace cat
{
    //
    // trait for concepts
    //

    template <typename M>
    struct is_monoid : std::false_type
    { };

    //
    // instance
    //

    template <typename M, typename F, typename M1, typename M2> struct MonoidInstance;

    //
    // free (callable) functions:
    //

    template <typename M>
    auto mempty()
    {
        static_assert(is_monoid<std::decay_t<M>>::value, "Type not a monoid!");
        return MonoidInstance<M, std::vector<M>, M, M>{}.mempty();
    }

    namespace details
    {
        using namespace placeholders;

        struct mappend_
        {
            using function_type = _m(_m, _m);

            template <typename M1, typename M2>
            auto operator()(M1 && a, M2 && b) const
            {
                static_assert(is_monoid<std::decay_t<M1>>::value, "Type not a monoid!");
                static_assert(is_monoid<std::decay_t<M2>>::value, "Type not a monoid!");

                using M = std::decay_t<M1>;

                return MonoidInstance<
                        M, std::vector<M>, M1, M2>{}.
                        mappend(std::forward<M1>(a),
                                std::forward<M2>(b));
            }
        };

        struct mconcat_
        {
            using function_type = _m(_C<_m>);

            template <typename F>
            auto operator()(F && f) const
            {
                using M = inner_type_t<std::decay_t<F>>;

                static_assert(is_monoid<std::decay_t<M>>::value, "Type not a monoid!");

                return MonoidInstance<M, F, M, M >{}.
                        mconcat(std::forward<F>(f));
            }
        };
    }

    constexpr auto mappend = details::mappend_{};
    constexpr auto mconcat = details::mconcat_{};

    //
    // type class Monoid
    //

    template <typename M>
    struct Monoid
    {
        template <typename  F, typename M1, typename M2>
        struct where
        {
            virtual M mempty() = 0;
            virtual M mappend(M1 &&, M2 &&) = 0;
            virtual M mconcat(F && xs)
            {
                auto r = mempty();

                for(auto && x : std::forward<F>(xs))
                    r = cat::mappend(std::move(r), forward_as<F>(x));

                return r;
            };
        };
    };

} // namespace cat

