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

#include <cat/functor.hpp>
#include <cat/type_traits.hpp>
#include <cat/functional.hpp>
#include <cat/container.hpp>
#include <cat/infix.hpp>

#include <utility>
#include <vector>
#include <list>

//
// Sugaring monads...
//

#define CAT_UNPACK_ARGS(...)        __VA_ARGS__
#define CAT_UNPACK(xs)              CAT_UNPACK_ARGS xs
#define DO(var, fun, cont)        ((fun) >>= [&](auto var) { return CAT_UNPACK(cont); })

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

        struct Mreturn_
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
                static_assert(meta::on_outer_type<is_monad,std::decay_t<Mx>>::value, "Type M not a monad!");

                using Ma = rebind_type_t<std::decay_t<Mx>, std::decay_t<A_>>;
                return MonadInstance<Ma, details::MreturnAs<Ma>, Ma, A_>{}.mreturn(std::forward<A_>(a));
            }
        };

        template <typename T>
        struct Return
        {
            T value;

            template < template <typename ...> class M>
            operator M<std::remove_reference_t<T>>()
            {
                return Mreturn_{}.in<M>(std::forward<T>(value));
            }
        };

        struct Mbind_
        {
            using function_type = _M<_b>(_M<_a> &&, _f<_M<_b>(_a)>);

            template <typename Ma_, typename Fun>
            auto operator()(Ma_ && ma, Fun f) const
            {
                using Ma = std::decay_t<Ma_>;
                static_assert(meta::on_outer_type<is_monad,std::decay_t<Ma>>::value, "Type M not a monad!");

                return MonadInstance<Ma, Fun, Ma_, inner_type_t<Ma> >{}.mbind(std::forward<Ma_>(ma), std::move(f));
            }
        };


    }

    constexpr auto mreturn = details::Mreturn_{};
    constexpr auto mbind = details::Mbind_ {};

    template <typename T>
    details::Return<T>
    mreturn_(T && value)
    {
        return details::Return<T>{ std::forward<T>(value) };
    }

    //
    // monad plus
    //

    template <typename  Ma>
    auto mzero()
    {
         static_assert(meta::on_outer_type<is_monad_plus,std::decay_t<Ma>>::value, "Type M not a monad plus!");
         return MonadPlusInstance<Ma, Ma, Ma>{}.mzero();
    }

    namespace details
    {
        using namespace placeholders;

        struct Mplus_
        {
            using function_type = _M<_a>(_M<_a> &&, _M<_a> &&);

            template <typename Ma_, typename Mb_>
            auto operator()(Ma_ && a, Mb_ && b) const
            {
                 using Ma = std::decay_t<Ma_>;

                 static_assert(meta::on_outer_type<is_monad_plus, Ma>::value, "Type M not a monad plus!");
                 return MonadPlusInstance<Ma, Ma_, Mb_>{}.mplus(std::forward<Ma_>(a), std::forward<Mb_>(b));
            }
        };
    }

    constexpr auto mplus = details::Mplus_ {};

    //
    // class Monad
    //

    template <template <typename ...> class M>
    struct Monad
    {
        template <typename Fun, typename A, typename Ma_, typename A_>
        struct where
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
        struct where
        {
            virtual M<A> mzero() = 0;
            virtual M<A> mplus(Ma_ &&, Mb_ &&)  = 0;
        };
    };

    //
    // operators
    //

    template <typename Ma_, typename Fun,
              std::enable_if_t<meta::on_outer_type<is_monad, std::decay_t<Ma_>>::value> * = nullptr>
    auto operator>>=(Ma_ && ma, Fun f)
    {
        return mbind(std::forward<Ma_>(ma), std::move(f));
    }

    template <typename Ma_, typename Mb_,
              std::enable_if_t<
                meta::on_outer_type<is_monad, std::decay_t<Ma_>>::value &&
                meta::on_outer_type<is_monad, std::decay_t<Mb_>>::value> * = nullptr>
    auto operator>>(Ma_ && ma, Mb_ && mb)
    {
        return mbind(std::forward<Ma_>(ma), constant(std::forward<Mb_>(mb)));
    }

    //
    // sequence
    //

    template <template <typename...> class C, typename Ma, typename ...Ts>
    auto sequence(C<Ma, Ts...> const &ms)
    {
        static_assert(meta::on_outer_type<is_monad_plus, Ma>::value, "Type M not a monad!");

#ifdef _LIBCPP_VERSION
        using Cont = C<inner_type_t<Ma>, std::allocator<inner_type_t<Ma>>>;
#else
        using Cont = C<inner_type_t<Ma>>;
#endif

        auto k = [] (auto m, auto ms)
        {
            return (m >>= [&](auto x) {
                    return (ms >>= [&] (auto xs) {
                            Cont l{x};

                            insert(l, std::begin(xs), std::end(xs));
                            return mreturn.as<Ma>(std::move(l));
                            });
                    });
        };

        return container::foldr(k, mreturn.as<Ma>(Cont{}), ms);
    }

    //
    // mapM and forM
    //

    template <typename Ma, typename F>
    auto mapM(F f, Ma && xs)
    {
        return sequence(fmap(f, std::forward<Ma>(xs)));
    }

    template <typename Ma, typename F>
    auto forM(Ma && xs, F f)
    {
        return mapM(f, std::forward<Ma>(xs));
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

    template <typename Ma, template <typename ...> class C, typename ...Ts>
    auto msum (C<Ma, Ts...> && xs)
    {
        auto acc = mzero<Ma>();
        for(auto & x : xs)
            acc = mplus(std::move(acc), std::move(x));
        return acc;
    }

    template <typename Ma, template <typename ...> class C, typename ...Ts>
    auto msum (C<Ma, Ts...> const & xs)
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

