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

#include <cat/show.hpp>
#include <cat/monoid/monoid.hpp>

#include <type_traits>

namespace cat
{
    //
    // Any Boolean
    //

    struct Any
    {
        bool value;
        explicit operator bool() const { return value; }
    };
    inline bool operator==(Any a, Any b) { return a.value == b.value; }
    inline bool operator!=(Any a, Any b) { return !(a == b); }

    template <>
    struct is_monoid<Any> : std::true_type { };


    template <typename F, typename M1, typename M2>
    struct MonoidInstance<Any, F, M1, M2> final : Monoid<Any>::
    template where<F, M1, M2>
    {
        virtual Any mempty() override
        {
            return Any{ false };
        }

        virtual Any mappend(M1 && a, M2 && b) override
        {
            return Any { a.value || b.value };
        }
    };


    template <>
    struct ShowInstance<Any> final : Show<Any>
    {
        std::string
        show(const Any &t) override
        {
            return std::string("Any ") + (t.value ? "true" : "false");
        }
    };

    //
    // All boolean
    //

    struct All
    {
        bool value;
        explicit operator bool() const { return value; }
    };
    inline bool operator==(All a, All b) { return a.value == b.value; }
    inline bool operator!=(All a, All b) { return !(a == b); }

    template <>
    struct is_monoid<All> : std::true_type { };


    template <typename F, typename M1, typename M2>
    struct MonoidInstance<All, F, M1, M2> final : Monoid<All>::
    template where<F, M1, M2>
    {
        virtual All mempty() override
        {
            return All{ true };
        }

        virtual All mappend(M1 && a, M2 && b) override
        {
            return All { a.value && b.value };
        }
    };

    template <>
    struct ShowInstance<All> final : Show<All>
    {
        std::string
        show(const All &t) override
        {
            return std::string("All ") + (t.value ? "true" : "false");
        }
    };

    //
    // Sum numeric
    //

    template <typename T>
    struct Sum
    {
        static_assert(std::is_arithmetic<T>::value, "Not arithmetic type");

        T value;
        explicit operator T() const { return value; }
    };

    template <typename T>
    Sum<T> sum(T value)
    {
        return Sum<T>{ value };
    }

    template <typename T>
    struct is_monoid<Sum<T>> : std::true_type { };


    template <typename T, typename F, typename M1, typename M2>
    struct MonoidInstance<Sum<T>, F, M1, M2> final : Monoid<Sum<T>>::
    template where<F, M1, M2>
    {
        virtual Sum<T> mempty() override
        {
            return Sum<T>{0};
        }

        virtual Sum<T> mappend(M1 && a, M2 && b) override
        {
            return Sum<T>{ forward_as<M1>(a.value) +
                           forward_as<M2>(b.value) };
        }
    };

    template <typename T>
    struct ShowInstance<Sum<T>> final : Show<Sum<T>>
    {
        std::string
        show(const Sum<T> &t) override
        {
            return type_name<Sum<T>>() + ' ' + show(t.value());
        }
    };

    //
    // Product numeric
    //

    template <typename T>
    struct Product
    {
        static_assert(std::is_arithmetic<T>::value, "Not arithmetic type");

        T value;
        explicit operator T() const { return value; }
    };

    template <typename T>
    Product<T> product(T value)
    {
        return Product<T>{ value };
    }

    template <typename T>
    struct is_monoid<Product<T>> : std::true_type { };


    template <typename T, typename F, typename M1, typename M2>
    struct MonoidInstance<Product<T>, F, M1, M2> final : Monoid<Product<T>>::
    template where<F, M1, M2>
    {
        virtual Product<T> mempty() override
        {
            return Product<T>{1};
        }

        virtual Product<T> mappend(M1 && a, M2 && b) override
        {
            return Product<T>{ forward_as<M1>(a.value) *
                               forward_as<M2>(b.value) };
        }
    };

    template <typename T>
    struct ShowInstance<Product<T>> final : Show<Product<T>>
    {
        std::string
        show(const Product<T> &t) override
        {
            return type_name<Product<T>>() + ' ' + show(t.value());
        }
    };
};
