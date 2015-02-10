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

#include <type_traits>
#include <cat/monoid/monoid.hpp>

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

    template <template <typename ...> class F>
    struct MonoidInstance<Any, F> : Monoid<Any>::template Class<F>
    {
        virtual Any mempty() final
        {
            return Any{ false };
        }

        virtual Any mappend(Any const &a, Any const &b) final
        {
            return Any { a.value || b.value };
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

    template <template <typename ...> class F>
    struct MonoidInstance<All, F> : Monoid<All>::template Class<F>
    {
        virtual All mempty() final
        {
            return All{ true };
        }

        virtual All mappend(All const &a, All const &b) final
        {
            return All { a.value && b.value };
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

    template <typename T, template <typename ...> class F>
    struct MonoidInstance<Sum<T>, F> : Monoid<Sum<T>>::template Class<F>
    {
        virtual Sum<T> mempty() final
        {
            return Sum<T>{0};
        }

        virtual Sum<T> mappend(Sum<T> const &a, Sum<T> const &b) final
        {
            return Sum<T>{ a.value + b.value };
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

    template <typename T, template <typename ...> class F>
    struct MonoidInstance<Product<T>, F> : Monoid<Product<T>>::template Class<F>
    {
        virtual Product<T> mempty() final
        {
            return Product<T>{1};
        }

        virtual Product<T> mappend(Product<T> const &a, Product<T> const &b) final
        {
            return Product<T>{ a.value * b.value };
        }
    };
};
