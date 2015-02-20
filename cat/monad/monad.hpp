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
#include <list>

#include <cat/type_traits.hpp>
#include <cat/functional.hpp>
#include <cat/infix.hpp>

namespace cat
{
    namespace details
    {
        template <template <typename ...> class M>
        struct Null
        {
            template <typename T>
            M<T> operator()(T const &) const
            {
                return M<T>{};
            }
        };
    }

    //
    // class Monad
    //

    template <template <typename ...> class M>
    struct Monad
    {
        template <typename Fun, typename A>
        struct Class
        {
            virtual auto mbind(M<A> ma, Fun f)
                -> M< inner_type_t<decltype(f(std::declval<A>()))> > = 0;

            virtual M<A> mreturn(A) = 0;
        };
    };

    template <template <typename ...> class M, typename ...> struct MonadInstance;

    template <template <typename ...> class M, typename A>
    auto mreturn(A a)
    {
        return MonadInstance<M, A, details::Null<M>>{}.mreturn(std::move(a));
    }

    template <template <typename ...> class M, typename Fun, typename A>
    auto mbind(M<A> ma, Fun f)
    {
        return MonadInstance<M, A, Fun>{}.mbind(std::move(ma), std::move(f));
    }

    template <template <typename ...> class M, typename Fun, typename A>
    auto operator>>=(M<A> ma, Fun f)
    {
        return mbind(std::move(ma), std::move(f));
    }

    template <template <typename ...> class M, typename A, typename B>
    auto operator>>(M<A> ma, M<B> mb)
    {
        return mbind(std::move(ma), constant(mb));
    }
    //
    // sequence
    //

    template <template <typename ...> class M, typename A>
    auto sequence(std::list<M<A>> const &ms)
    {
        auto k = [] (auto m, auto ms)
        {
            return (m >>= [&](auto x) {
                return (ms >>= [&] (auto xs) {

                    std::list<A> l{x};

                    l.insert(l.end(), std::begin(xs), std::end(xs));

                    return mreturn<M>(std::move(l));
                });
            });
        };

        return foldr(k, mreturn<M>( std::list<A>{} ), ms);
    }


    //
    // Kelisli composition
    //

    template <typename F, typename G>
    struct Kleisli_
    {
        template <template <typename ...> class M, typename _, typename A>
        static constexpr auto return_to(M<_>, A a)
        {
            return mreturn<M>(std::move(a));
        }

        template <typename A>
        constexpr auto operator()(A a) const
        {
            return ( return_to(return_type_t<F>{}, std::move(a)) >>= f_ ) >>= g_;
        }

        F f_;
        G g_;
    };


    struct kleisli_
    {
        template <typename F, typename G>
        constexpr auto operator()(F f, G g) const
        {
            return Kleisli_<F,G>{ std::move(f), std::move(g) };
        }
    };

    constexpr auto k = infix(kleisli_{});


    template <template <typename...> class F>
    struct is_monad : std::false_type
    { };


} // namespace cat

