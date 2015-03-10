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
#include <cat/container.hpp>
#include <cat/infix.hpp>


//
// Sugaring monads...
//

#define CAT_UNPACK_ARGS(...)        __VA_ARGS__
#define CAT_UNPACK(xs)              CAT_UNPACK_ARGS xs
#define LET(var, fun, cont)        ((fun) >>= [&](auto var) { return CAT_UNPACK(cont); })

namespace cat
{
    //
    // trait for concepts
    //

    template <template <typename...> class F>
    struct is_monad : std::false_type
    { };

    template <template <typename...> class F>
    struct is_monad_plus : std::false_type
    { };

    //
    // instances
    //

    template <typename Ma, typename ...> struct MonadInstance;
    template <typename Ma, typename ...> struct MonadPlusInstance;


    namespace details
    {
        //
        // callable version of mreturn...
        //

        template <template <typename ...> class M>
        struct Mreturn
        {
            template <typename T>
            M<T> operator()(T && value) const;
        };

        template <class Mx>
        struct MreturnAs
        {
            template <typename T>
            auto operator()(T && value) const -> rebind_type_t<std::decay_t<Mx>, std::decay_t<T>>;
        };
    }


    //
    // free functions
    //

    namespace details
    {
        using namespace placeholders;

        struct mreturn_
        {
            template <template <typename ...> class M, typename A_>
            auto in(A_ && a) const
            {
                 static_assert(is_monad<M>::value, "Type M not a monad!");
                 using A = std::decay_t<A_>;
                 return MonadInstance<M<A>, details::Mreturn<M>, M<A>, A_>{}.mreturn(std::forward<A_>(a));
            }

            template <typename Mx, typename A_>
            auto as (A_ && a) const
            {
                static_assert(on_outer_type<is_monad,std::decay_t<Mx>>::value, "Type M not a monad!");

                using Ma = rebind_type_t<std::decay_t<Mx>, std::decay_t<A_>>;
                return MonadInstance<Ma, details::MreturnAs<Ma>, Ma, A_>{}.mreturn(std::forward<A_>(a));
            }
        };

        struct mbind_
        {
            using function_type = _M<_b>(_M<_a> &&, _<_M<_b>(_a)>);

            template <typename Ma_, typename Fun>
            auto operator()(Ma_ && ma, Fun f) const
            {
                using Ma = std::decay_t<Ma_>;
                static_assert(on_outer_type<is_monad,std::decay_t<Ma>>::value, "Type M not a monad!");

                return MonadInstance<Ma, Fun, Ma_, inner_type_t<Ma> >{}.mbind(std::forward<Ma_>(ma), std::move(f));
            }
        };
    }

    constexpr auto mreturn = details::mreturn_{};
    constexpr auto mbind = details::mbind_ {};

    //
    // monad plus
    //

    template <typename  Ma>
    auto mzero()
    {
         static_assert(on_outer_type<is_monad_plus,std::decay_t<Ma>>::value, "Type M not a monad plus!");
         return MonadPlusInstance<Ma, Ma, Ma>{}.mzero();
    }

    namespace details
    {
        using namespace placeholders;

        struct mplus_
        {
            using function_type = _M<_a>(_M<_a> &&, _M<_a> &&);

            template <typename Ma_, typename Mb_>
            auto operator()(Ma_ && a, Mb_ && b) const
            {
                 using Ma = std::decay_t<Ma_>;

                 static_assert(on_outer_type<is_monad_plus, Ma>::value, "Type M not a monad plus!");
                 return MonadPlusInstance<Ma, Ma_, Mb_>{}.mplus(std::forward<Ma_>(a), std::forward<Mb_>(b));
            }
        };
    }

    constexpr auto mplus = details::mplus_ {};

    //
    // class Monad
    //

    template <template <typename ...> class M>
    struct Monad
    {
        template <typename Fun, typename A, typename Ma_, typename A_>
        struct _
        {
            virtual auto mbind(Ma_ && ma, Fun f) -> std::result_of_t<Fun(A)> = 0;

            virtual M<A> mreturn(A_ &&) = 0;
        };
    };

    //
    // class Monad Plus
    //

    template <template <typename ...> class M>
    struct MonadPlus : Monad<M>
    {
        template <typename A, typename Ma_, typename Mb_>
        struct _
        {
            virtual M<A> mzero() = 0;
            virtual M<A> mplus(Ma_ &&, Mb_ &&)  = 0;
        };
    };

    //
    // operators
    //

    template <template <typename ...> class M, typename Fun, typename A,
              std::enable_if_t<is_monad<M>::value> * = nullptr>
    auto operator>>=(M<A> && ma, Fun f)
    {
        return mbind(std::move(ma), std::move(f));
    }
    template <template <typename ...> class M, typename Fun, typename A,
              std::enable_if_t<is_monad<M>::value> * = nullptr>
    auto operator>>=(M<A> const &ma, Fun f)
    {
        return mbind(ma, std::move(f));
    }


    template <template <typename ...> class M, typename A, typename B,
              std::enable_if_t<is_monad<M>::value> * = nullptr>
    auto operator>>(M<A> && ma, M<B> const & mb)
    {
        return mbind(std::move(ma), constant(mb));
    }
    template <template <typename ...> class M, typename A, typename B,
              std::enable_if_t<is_monad<M>::value> * = nullptr>
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
                            return mreturn.in<M>(std::move(l));
                            });
                    });
        };

        return foldr(k, mreturn.in<M>( std::list<A>{} ), ms);
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

    namespace details
    {
        template <typename F, typename G>
        struct Kleisli_
        {
            template <typename A>
            constexpr auto operator()(A && a) const
            {
                return (mreturn.as<return_type_t<F>>(std::forward<A>(a)) >>= f_ ) >>= g_;
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
    }

    constexpr auto kleisli = infix(details::kleisli_{});

    //
    // join
    //

    template <typename MMa>
    auto join(MMa && x)
    {
        return  std::forward<MMa>(x) >>= identity;
    }

    //
    // msum
    //

    template <template <typename ...> class F, typename Ma>
    auto msum (F<Ma> && xs)
    {
        auto acc = mzero<Ma>();
        for(auto & x : xs)
            acc = mplus(std::move(acc), std::move(x));
        return acc;
    }

    template <template <typename ...> class F, typename Ma>
    auto msum (F<Ma> const & xs)
    {
        auto acc = mzero<Ma>();
        for(auto & x : xs)
            acc = mplus(std::move(acc), x);
        return acc;
    }

    //
    // guard
    //

    template <typename Ma>
    auto guard(bool value)
    {
        if (value)
            return mreturn.as<Ma>(0);
        return mzero<Ma>();
    }

    namespace details
    {
        template <template <typename ...> class M>
        template <typename T>
        M<T> Mreturn<M>::operator()(T && value) const
        {
            return mreturn.in<M>(std::forward<T>(value));
        };

        template <class Mx>
        template <typename T>
        auto MreturnAs<Mx>::operator()(T && value) const -> rebind_type_t<std::decay_t<Mx>, std::decay_t<T>>
        {
            return mreturn.as<Mx>(std::forward<T>(value));
        };

    } // namespace details

} // namespace cat

