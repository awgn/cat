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

#include <forward_list>

#include <cat/iterator.hpp>
#include <cat/functor/functor.hpp>


namespace cat
{
    // forward_list is a functor:
    //

    template <> struct is_functor<std::forward_list> : std::true_type { };

    // forward_list instance:
    //

    namespace functor_forward_list
    {
        template <typename Fun, typename Functor>
        auto fmap(Fun f, Functor && xs)
        {
            using B = decltype(f(forward_as<Functor>(xs.front())));

            std::forward_list<B> out
            {
                map_iterator( auto_begin(xs), f ),
                map_iterator( auto_end(xs), f )
            };

            return out;
        }
    };


    template <typename Fun, typename A>
    struct FunctorInstance<std::forward_list<A> const &, Fun> final : Functor<std::forward_list<A> const &>::
    template _<Fun>
    {
        using B = typename std::result_of<Fun(A)>::type;

        std::forward_list<B>
        fmap(Fun f, std::forward_list<A> const & xs) override
        {
            return functor_forward_list::fmap(std::move(f), std::move(xs));
        }
    };

    template <typename Fun, typename A>
    struct FunctorInstance<std::forward_list<A> &&, Fun> final : Functor<std::forward_list<A> &&>::
    template _<Fun>
    {
        using B = typename std::result_of<Fun(A)>::type;

        std::forward_list<B>
        fmap(Fun f, std::forward_list<A> && xs) override
        {
            return functor_forward_list::fmap(std::move(f), std::move(xs));
        }
    };


} // namespace cat

