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
#include <type_traits>

namespace cat
{
    template <typename T>
    class has_rebind
    {
        typedef char yes;
        typedef long no;

        template <class C> static yes check(typename C::template rebind<int>::other *);
        template <class C> static no  check(...);

    public:
        enum { value = sizeof(check<T>(0)) == sizeof(yes) };
    };


    template <typename T>
    struct is_default_deleter
    {
        enum { value = false };
    };
    template <typename T>
    struct is_default_deleter<std::default_delete<T>>
    {
        enum { value = true };
    };
    template <typename T>
    struct is_default_deleter<std::default_delete<T[]>>
    {
        enum { value = true };
    };


    template <typename T, typename To, typename = void>
    struct rebind
    {
        using type = T;
    };

    // rebind for default deleter...
    //
    template <typename T, typename To>
    struct rebind<T, To, typename std::enable_if<is_default_deleter<T>::value>::type>
    {
        using type = std::default_delete<To>;
    };

    // rebind for allocators...
    //
    template <typename T, typename To>
    struct rebind<T, To, typename std::enable_if<has_rebind<T>::value>::type>
    {
        using type = typename T::template rebind<To>::other;
    };

    template <typename ...Ts>
    using rebind_t = typename rebind<Ts...>::type;

    //
    // callable_arity...
    //

    template <typename T>
    struct callable_arity : callable_arity<decltype(&T::operator())> { };

    // operator() const
    template <typename F, typename R, typename ...Ts>
    struct callable_arity<R(F::*)(Ts...) const>
    {
        enum { value = sizeof...(Ts) };
    };
    // operator()
    template <typename F, typename R, typename ...Ts>
    struct callable_arity<R(F::*)(Ts...)>
    {
        enum : size_t { value = sizeof...(Ts) };
    };
    // function
    template <typename R, typename ...Ts>
    struct callable_arity<R(Ts...)>
    {
        enum { value = sizeof...(Ts) };
    };
    // function pointer
    template <typename R, typename ...Ts>
    struct callable_arity<R(*)(Ts...)>
    {
        enum { value = sizeof...(Ts) };
    };

}
