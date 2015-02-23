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
#include <vector>

#include <cat/functor.hpp>
#include <cat/type_traits.hpp>
#include <cat/functional.hpp>
#include <cat/infix.hpp>

namespace cat
{

    template <template <typename ...> class M, typename A_>
    auto mreturn(A_ && a);

    //
    // class Monad
    //

    template <template <typename ...> class M>
    struct Monad
    {
        struct Identity
        {
            template <typename T>
            M<T> operator()(T && value) const
            {
                return cat::mreturn<M>(std::forward<T>(value));
            }
        };

        template <typename Fun, typename A, typename Ma_, typename A_>
        struct _
        {
            virtual auto mbind(Ma_ && ma, Fun f) -> std::result_of_t<Fun(A)> = 0;

            virtual M<A> mreturn(A_ &&) = 0;
        };
    };

    //
    // instance
    //

    template <typename Ma, typename ...> struct MonadInstance;

    //
    // free functions
    //


    template <template <typename ...> class M, typename A_>
    auto mreturn(A_ && a)
    {
         using A = std::decay_t<A_>;

         return MonadInstance<M<A>, typename Monad<M>::Identity, M<A>, A_>{}.mreturn(std::forward<A_>(a));
    }


    template <template <typename ...> class M, typename _, typename A>
    constexpr auto mreturn_to(M<_>, A && a)
    {
        return mreturn<M>(std::forward<A>(a));
    }

    template <typename Ma_, typename Fun>
    auto mbind(Ma_ && ma, Fun f)
    {
        using Ma = std::decay_t<Ma_>;

        return MonadInstance<Ma, Fun, Ma_, inner_type_t<Ma> >{}.mbind(std::forward<Ma_>(ma), std::move(f));
    }

    //
    // operators
    //

    template <template <typename ...> class M, typename Fun, typename A>
    auto operator>>=(M<A> && ma, Fun f)
    {
        return mbind(std::move(ma), std::move(f));
    }
    template <template <typename ...> class M, typename Fun, typename A>
    auto operator>>=(M<A> const &ma, Fun f)
    {
        return mbind(ma, std::move(f));
    }


    template <template <typename ...> class M, typename A, typename B>
    auto operator>>(M<A> && ma, M<B> const & mb)
    {
        return mbind(std::move(ma), constant(mb));
    }
    template <template <typename ...> class M, typename A, typename B>
    auto operator>>(M<A> const & ma, M<B> const & mb)
    {
        return mbind(ma, constant(mb));
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
    // mapM and forM
    //

    template <typename A, typename F>
    auto mapM(F f, std::list<A> const &xs)
    {
        return sequence( fmap(f, xs) );
    }

    template <typename A, typename F>
    auto forM(std::list<A> const &ma, F f)
    {
        return mapM(f, ma);
    }

    //
    // Kelisli composition
    //

    template <typename F, typename G>
    struct Kleisli_
    {
        template <typename A>
        constexpr auto operator()(A && a) const
        {
            return (mreturn_to(return_type_t<F>{}, std::forward<A>(a)) >>= f_ ) >>= g_;
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

    //
    // trait for concepts
    //

    template <template <typename...> class F>
    struct is_monad : std::false_type
    { };


} // namespace cat

