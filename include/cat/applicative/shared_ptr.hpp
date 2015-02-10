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

#include <memory>

#include <cat/functor/shared_ptr.hpp>
#include <cat/applicative/applicative.hpp>

namespace cat
{
    // shared_ptr is an applicative instance:
    //

    template <> struct is_applicative<std::shared_ptr> : std::true_type { };

    // shared_ptr instance:
    //

    template <typename Fun, typename A, typename ...Ts>
    struct ApplicativeInstance<std::shared_ptr, Fun, A, Ts...> : Applicative<std::shared_ptr>::Class<Fun, A, Ts...>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        std::shared_ptr<A>
        pure(A const &elem) final
        {
            return std::make_shared<A>(elem);
        }

        std::shared_ptr<B>
        apply(std::shared_ptr<Fun> const &f, std::shared_ptr<A> const &x) final
        {
            if (f && x)
                return std::make_shared<B>((*f)(*x));

            return std::shared_ptr<B>{};
        }
    };

} // namespace cat

