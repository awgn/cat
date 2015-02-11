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

#include <utility>
#include <cat/functor/functor.hpp>
#include <cat/infix.hpp>

namespace cat
{
    //
    // class Applicative:
    //

    template <template <typename...> class F>
    struct Applicative
    {
        template <typename Fun, typename A, typename ...Ts>
        struct Class
        {
            virtual auto pure(A const &) -> F<A, Ts...> = 0; // lift a value.
            virtual auto apply(F<Fun> const &, F<A, Ts...> const &) -> F<decltype(std::declval<Fun>()(std::declval<A>()))>
            { throw nullptr; }
        };
    };


    template <template <typename ...> class F, typename ...> struct ApplicativeInstance;

    template <template <typename ...> class F, typename A>
    auto pure(A const &value)
    {
        return ApplicativeInstance<F, Id, A>{}.pure(value);
    }

    template <template <typename ...> class F, typename Fun, typename A, typename ...Ts>
    auto apply(F<Fun> const &fs, F<A, Ts...> const &xs)
    {
        return ApplicativeInstance<F, Fun, A, Ts...>{}.apply(fs, xs);
    }

    //
    // infix adaptors...
    //

    template <template <typename ...> class F, typename Fun, typename A, typename ...Ts>
    auto operator*(F<Fun> const &fs, F<A, Ts...> const &xs)
    {
        return apply(fs, xs);
    }

    namespace
    {
        struct app
        {
            template <typename L, template <typename ...> class F, typename A>
            auto operator()(L const &lhs, F<A> const &rhs) const
            {
                return apply(F<L>{lhs}, rhs);
            }
        };

        infix_adaptor<app> $;
    }

    //
    // lift...
    //

    template <template <typename ...> class F, typename Fun, typename A, typename ...Ts>
    auto liftA(Fun const &f, F<A, Ts...> const &xs)
    {
        std::function< decltype(f(std::declval<A>()))(A) > f_(f);
        return pure<F>(f_) * xs;
    }


    template <template <typename ...> class A>
    struct is_applicative : std::false_type
    { };


} // namespace cat

