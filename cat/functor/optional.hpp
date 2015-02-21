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

#include <experimental/optional>
#include <cat/functor/functor.hpp>

namespace cat
{
    // optional is a functor:
    //

    template <> struct is_functor<std::experimental::optional> : std::true_type { };

    // experimental::optional instance:
    //

    namespace functor_optional
    {
        template <typename Fun, typename Functor>
        static auto fmap(Fun f, Functor && x)
        {
            using type = decltype(f(forward_as<Functor>(*x)));

            if (x)
                return std::experimental::make_optional(f(forward_as<Functor>(*x)));

            return std::experimental::optional<type>();
        }
    };


    template <typename Fun, typename A>
    struct FunctorInstance<std::experimental::optional<A> const &, Fun> final : Functor<std::experimental::optional<A> const &>::
    template _<Fun>
    {
        using B = typename std::result_of<Fun(A)>::type;

        std::experimental::optional<B>
        fmap(Fun f, std::experimental::optional<A> const & xs) override
        {
            return functor_optional::fmap(std::move(f), xs);
        }
    };

    template <typename Fun, typename A>
    struct FunctorInstance<std::experimental::optional<A> &&, Fun> final : Functor<std::experimental::optional<A> &&>::
    template _<Fun>
    {
        using B = typename std::result_of<Fun(A)>::type;

        std::experimental::optional<B>
        fmap(Fun f, std::experimental::optional<A> && xs) override
        {
            return functor_optional::fmap(std::move(f), std::move(xs));
        }
    };

} // namespace cat

