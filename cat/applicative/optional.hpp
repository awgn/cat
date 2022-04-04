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

#include <cat/optional.hpp>
#include <cat/applicative/applicative.hpp>

#include <optional>

namespace cat
{
    // optional is an applicative instance:
    //

    template <> struct is_applicative<std::optional> : std::true_type { };

    // optional instance:
    //

    template <typename F, typename A, typename Ff_, typename Fa_, typename A_>
    struct ApplicativeInstance<std::optional<F>, std::optional<A>, Ff_, Fa_, A_>  final : Applicative<std::optional>::
    template where<F, A, Ff_, Fa_, A_>
    {
        using B = std::invoke_result_t<F, A_>;

        std::optional<A>
        pure(A_ &&elem) override
        {
            return std::make_optional(std::forward<A_>(elem));
        }

        std::optional<B>
        apply(Ff_ && f, Fa_ &&x) override
        {
            if (f && x)
                return std::make_optional<B>(forward_as<Ff_>(*f)(forward_as<Fa_>(*x)));

            return {};
        }

    };

    // optional is an alternative instance:
    //

    template <> struct is_alternative<std::optional> : std::true_type { };

    // optional instance:
    //

    template <typename A, typename Fl_, typename Fr_>
    struct AlternativeInstance<std::optional<A>, Fl_, Fr_> final : Alternative<std::optional>::
    template where<std::optional<A>, Fl_, Fr_>
    {
        std::optional<A>
        empty() override
        {
            return std::optional<A>{};
        }

        std::optional<A>
        or_(Fl_ && lhs, Fr_ && rhs) override
        {
            if (lhs)
                return std::forward<Fl_>(lhs);
            return std::forward<Fr_>(rhs);
        }
    };

} // namespace cat
