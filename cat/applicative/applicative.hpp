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

#include <cat/utility.hpp>
#include <cat/type_traits.hpp>
#include <cat/functional.hpp>
#include <cat/infix.hpp>

#include <utility>

namespace cat
{
    //
    // class applicative:
    //

    template <template <typename...> class F>
    struct Applicative
    {
        template <typename Fun, typename A, typename Ff_, typename Fa_, typename A_>
        struct where
        {
            virtual auto pure(A_ &&) -> F<A> = 0; // lift a value.
            virtual auto apply(Ff_ &&, Fa_ &&) -> F<std::result_of_t<Fun(A_)>> = 0;
        };
    };


    //
    // Instance
    //

    template <typename FF, typename FA, typename ...> struct ApplicativeInstance;

    namespace details
    {
        using namespace placeholders;

        struct pure_
        {
            using function_type = _F<_a>(_a);

            template <template <typename ...> class F, typename A_>
            auto in(A_ && value) const
            {
                using Fa = F<std::decay_t<A_>>;
                return ApplicativeInstance< F<Identity>, Fa, F<Identity>, Fa, A_>{}.pure(std::forward<A_>(value));
            }

            template <typename Fx, typename A_>
            auto as(A_ && value) const
            {
                using Fa = rebind_type_t<std::decay_t<Fx>, std::decay_t<A_>>;
                using Ff = rebind_type_t<std::decay_t<Fx>, Identity>;

                return ApplicativeInstance<Ff, Fa, Ff, Fa, A_>{}.pure(std::forward<A_>(value));
            }

        };

        struct apply_
        {
            using function_type = _F<_b>( _F<_f<_b(_a)>>, _F<_a>);

            template <typename Ff_, typename Fa_>
            auto operator()(Ff_ && fs, Fa_ &&xs) const
            {
                using Ff = std::decay_t<Ff_>;
                using Fa = std::decay_t<Fa_>;

                return ApplicativeInstance<Ff, Fa, Ff_, Fa_, int>{}.apply(std::forward<Ff_>(fs), std::forward<Fa_>(xs));
            }
        };
    }

    //
    // trait for concepts
    //

    template <template <typename ...> class A>
    struct is_applicative : std::false_type
    { };

    //
    // functions: pure and apply
    //

    constexpr auto pure = details::pure_ {};
    constexpr auto apply = details::apply_ {};

    //
    // operator *
    //

    template <typename Ff_, typename Fa_,
              std::enable_if_t<
                    meta::on_outer_type<is_applicative, std::decay_t<Ff_>>::value &&
                    meta::on_outer_type<is_applicative, std::decay_t<Fa_>>::value
              > * = nullptr>
    auto operator*(Ff_  && fs, Fa_ &&xs)
    {
        return apply(std::forward<Ff_>(fs), std::forward<Fa_>(xs));
    }

    //
    // infix operator: f <$>
    //

    namespace details
    {
        using namespace placeholders;

        struct fapply_
        {
            using function_type = _F<_b>(_f<_b(_a)>, _F<_a>);

            template <typename F, typename Fa>
            auto operator()(F && f, Fa && xs) const
            {
                return apply(pure.as<std::decay_t<Fa>>(std::forward<F>(f)), std::forward<Fa>(xs));
            }
        };
    }

    constexpr infix_adaptor<details::fapply_> $;


    //
    // class Alternative:
    //

    template <template <typename...> class F>
    struct Alternative : Applicative<F>
    {
        template <typename Fa, typename Fl_, typename Fr_>
        struct where
        {
            virtual auto empty() -> Fa = 0;   // identity of or_
            virtual auto or_(Fl_ &&, Fr_ &&) -> Fa = 0;
        };
    };

    //
    // Alternative instance
    //

    template <typename A, typename ...> struct AlternativeInstance;

    //
    // free functions...
    //

    template <typename Fa>
    auto empty()
    {
        return AlternativeInstance<Fa, Fa, Fa>{}.empty();
    }


    namespace details
    {
        using namespace placeholders;

        struct or_
        {
            using function_type = _F<_a>(_F<_a>, _F<_a>);

            template <typename Fl_, typename Fr_>
            auto operator()(Fl_&& lhs, Fr_ && rhs) const
            {
                static_assert(std::is_same<std::decay_t<Fl_>, std::decay_t<Fr_>>::value, "alternative::or_ types differ");

                using Fa = std::decay_t<Fl_>;

                return AlternativeInstance<Fa, Fl_, Fr_>{}.or_(std::forward<Fl_>(lhs),
                                                               std::forward<Fr_>(rhs));
            }
        };
    }

    constexpr auto or_ = details::or_ {};

    //
    // trait for concepts
    //

    template <template <typename ...> class A>
    struct is_alternative : std::false_type
    { };


    template <typename Fl, typename Fr,
              std::enable_if_t<meta::on_outer_type<is_alternative, Fl>::value> * = nullptr >
    inline auto operator||(Fl && lhs, Fr && rhs)
    {
        return or_(std::forward<Fl>(lhs), std::forward<Fr>(rhs));
    }

} // namespace cat

