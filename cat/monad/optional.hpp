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
#include <cat/monad/monad.hpp>
#include <type_traits>

namespace cat
{
    // std::optional is a monad:
    //

    template <> struct is_monad<std::optional> : std::true_type { };

    // std::optional instance:
    //

    template <typename A, typename Fun, typename Ma_, typename A_>
    struct MonadInstance<std::optional<A>, Fun, Ma_, A_> final : Monad<std::optional>::
    template where<Fun, A, Ma_, A_>
    {
        using B = inner_type_t<std::invoke_result_t<Fun, A>>;

        std::optional<B>
        mbind(Ma_ && x, Fun f) override
        {
            if (!x)
                return {};
            return f(forward_as<Ma_>(*x));
        }

        std::optional<A>
        mreturn(A_ && a) override
        {
            return {std::forward<A_>(a)};
        }

    };

    // std::optional is a monad_plus:
    //

    template <> struct is_monad_plus<std::optional> : std::true_type { };

    // std::optional instance:
    //

    template <typename A, typename Ma_, typename Mb_>
    struct MonadPlusInstance<std::optional<A>, Ma_, Mb_> final : MonadPlus<std::optional>::
    template where<A, Ma_, Mb_>
    {
        std::optional<A>
        mzero() override
        {
            return std::nullopt;
        }

        std::optional<A>
        mplus(Ma_ && a, Mb_ && b) override
        {
            if (a)
                return std::forward<Mb_>(a);
            return std::forward<Mb_>(b);
        }
    };

} // namespace cat
