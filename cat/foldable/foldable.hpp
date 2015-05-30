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

#include <cat/monoid.hpp>
#include <cat/utility.hpp>
#include <cat/type_traits.hpp>
#include <cat/placeholders.hpp>

namespace cat
{
    //
    // trait for concepts
    //

    template <template <typename...> class F>
    struct is_foldable : std::false_type
    { };


    // class foldable
    //

    template <template <typename ...> class F>
    struct Foldable
    {
        template <typename A, typename B, typename FunR, typename FunL, typename Fun, typename Fa_>
        struct where
        {
            virtual auto foldr (FunR fun, B, Fa_ && fa) -> B = 0;
            virtual auto foldl (FunL fun, B, Fa_ && fa) -> B = 0;
            virtual auto foldr1(Fun fun, Fa_ && fa) -> A = 0;
            virtual auto foldl1(Fun fun, Fa_ && fa) -> A = 0;
        };
    };

    //
    // instance
    //

    template <typename F, typename ...> struct FoldableInstance;

    //
    // function: foldl, foldr
    //

    namespace details
    {
        using namespace placeholders;

        struct foldr_
        {
            using function_type = _b(_f<_b(_a, _b)>, _b, _F<_a> &&);

            template <typename FunR, typename B, typename Fa_>
            auto operator()(FunR f, B value, Fa_ && xs) const
            {
                using Fa = std::decay_t<Fa_>;
                using A  = inner_value_type_t<Fa>;

                static_assert(meta::on_outer_type<is_foldable, Fa>::value, "Type not a foldable!");

                return FoldableInstance<Fa, B,
                       FunR,
                       std::function<B(B,A)>,
                       std::function<A(A,A)>,
                       Fa_>{}.foldr(std::move(f), std::move(value), std::forward<Fa_>(xs));
            }
        };

        struct foldr1_
        {
            using function_type = _a(_f<_a(_a, _a)>, _F<_a> &&);

            template <typename Fun, typename Fa_>
            auto operator()(Fun f, Fa_ && xs) const
            {
                using Fa = std::decay_t<Fa_>;
                using A  = inner_value_type_t<Fa>;

                static_assert(meta::on_outer_type<is_foldable, Fa>::value, "Type not a foldable!");

                return FoldableInstance<Fa, A,
                       std::function<A(A,A)>,
                       std::function<A(A,A)>,
                       Fun, Fa_>{}.foldr1(std::move(f), std::forward<Fa_>(xs));
            }
        };

        struct foldl_
        {
            using function_type = _b(_f<_b(_b, _a)>, _b, _F<_a> &&);

            template <typename FunL, typename B, typename Fa_>
            auto operator()(FunL f, B value, Fa_ && xs) const
            {
                using Fa = std::decay_t<Fa_>;
                using A  = inner_value_type_t<Fa>;

                static_assert(meta::on_outer_type<is_foldable, Fa>::value, "Type not a foldable!");

                return FoldableInstance<Fa, B,
                       std::function<B(A,B)>,
                       FunL,
                       std::function<A(A,A)>,
                       Fa_>{}.foldl(std::move(f), std::move(value), std::forward<Fa_>(xs));
            }
        };

        struct foldl1_
        {
            using function_type = _a(_f<_a(_a, _a)>, _F<_a> &&);

            template <typename Fun, typename Fa_>
            auto operator()(Fun f, Fa_ && xs) const
            {
                using Fa = std::decay_t<Fa_>;
                using A  = inner_value_type_t<Fa>;

                static_assert(meta::on_outer_type<is_foldable, Fa>::value, "Type not a foldable!");
                return FoldableInstance<Fa, A,
                        std::function<A(A,A)>,
                        std::function<A(A,A)>,
                        Fun,
                        Fa_>{}.foldl1(std::move(f), std::forward<Fa_>(xs));
            }
        };
    }

    constexpr auto foldr = details::foldr_{};
    constexpr auto foldl = details::foldl_{};
    constexpr auto foldr1 = details::foldr1_{};
    constexpr auto foldl1 = details::foldl1_{};

    //
    // additional functions...
    //

    namespace details
    {
        struct fold_
        {
            template <typename Fm_>
            auto operator()(Fm_ && xs) const
            {
                return foldr1(mappend, std::forward<Fm_>(xs));
            }
        };

        struct foldMap_
        {
            template <typename Fun, typename Fm_>
            auto operator()(Fun f, Fm_ && xs) const
            {
                using Fm = std::decay_t<Fm_>;
                using  B = decltype(f(std::declval<inner_value_type_t<Fm>>()));
                return foldr(compose(mappend, f), mempty<B>(), std::forward<Fm_>(xs));
            }
        };
    }

    constexpr auto fold = details::fold_{};
    constexpr auto foldMap = details::foldMap_{};

} // namespace cat

