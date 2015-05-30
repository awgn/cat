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

#include <cat/foldable/foldable.hpp>
#include <cat/utility.hpp>
#define CAT_FOLDABLE
#include <cat/container.hpp>

#include <set>

namespace cat
{
    // set is a foldable:
    //

    template <> struct is_foldable<std::set> : std::true_type { };

    // set instance:
    //

    template <typename A, typename B, typename FunR, typename FunL, typename Fun, typename Fa_>
    struct FoldableInstance<std::set<A>, B, FunR, FunL, Fun, Fa_> final : Foldable<std::set>::
    template where<A, B, FunR, FunL, Fun, Fa_>
    {
        B foldr(FunR f, B value, Fa_ && xs) override
        {
            return container::foldr(std::move(f), std::move(value), std::forward<Fa_>(xs));
        }

        B foldl(FunL f, B value, Fa_ && xs) override
        {
            return container::foldl(std::move(f), std::move(value), std::forward<Fa_>(xs));
        }

        A foldr1(Fun f, Fa_ && xs) override
        {
            return container::foldr1(std::move(f), std::forward<Fa_>(xs));
        }

        A foldl1(Fun f, Fa_ && xs) override
        {
            return container::foldl1(std::move(f), std::forward<Fa_>(xs));
        }
    };

    // multiset is a foldable:
    //

    template <> struct is_foldable<std::multiset> : std::true_type { };

    // multiset instance:
    //

    template <typename A, typename B, typename FunR, typename FunL, typename Fun, typename Fa_>
    struct FoldableInstance<std::multiset<A>, B, FunR, FunL, Fun, Fa_> final : Foldable<std::multiset>::
    template where<A, B, FunR, FunL, Fun, Fa_>
    {
        B foldr(FunR f, B value, Fa_ && xs) override
        {
            return container::foldr(std::move(f), std::move(value), std::forward<Fa_>(xs));
        }

        B foldl(FunL f, B value, Fa_ && xs) override
        {
            return container::foldl(std::move(f), std::move(value), std::forward<Fa_>(xs));
        }

        A foldr1(Fun f, Fa_ && xs) override
        {
            return container::foldr1(std::move(f), std::forward<Fa_>(xs));
        }

        A foldl1(Fun f, Fa_ && xs) override
        {
            return container::foldl1(std::move(f), std::forward<Fa_>(xs));
        }
    };

} // namespace cat

