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
    //////////////////////////////////////////////////////////////////////////////////
    //
    // has_rebind
    //

    template <typename T>
    class has_rebind
    {
        template <class C> static void check(typename C::template rebind<int>::other *) noexcept;
        template <class C> static void check(...) noexcept(false);
    public:

        enum { value = noexcept(check<T>(0)) };
    };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_default_deleter
    //

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

    //////////////////////////////////////////////////////////////////////////////////
    //
    //  generic rebind
    //

    template <typename T, typename To, typename = void>
    struct rebind
    {
        using type = T;
    };

    // rebind for default deleter...
    //
    template <typename T, typename To>
    struct rebind<T, To, std::enable_if_t<is_default_deleter<T>::value>>
    {
        using type = std::default_delete<To>;
    };

    // rebind for allocators...
    //
    template <typename T, typename To>
    struct rebind<T, To, std::enable_if_t<has_rebind<T>::value>>
    {
        using type = typename T::template rebind<To>::other;
    };

    template <typename ...Ts>
    using rebind_t = typename rebind<Ts...>::type;


    //////////////////////////////////////////////////////////////////////////////////
    //
    // callable_traits
    //

    template <typename T>
    struct callable_traits : callable_traits<decltype(&T::operator())> { };

    template <typename F, typename R, typename ...Ts>
    struct callable_traits<R(F::*)(Ts...) const>
    {
        using type = R(Ts...);
        enum : size_t { arity = sizeof...(Ts) };
    };
    template <typename F, typename R, typename ...Ts>
    struct callable_traits<R(F::*)(Ts...)>
    {
        using type = R(Ts...);
        enum : size_t { arity = sizeof...(Ts) };
    };
    template <typename R, typename ...Ts>
    struct callable_traits<R(Ts...)>
    {
        using type = R(Ts...);
        enum : size_t { arity = sizeof...(Ts) };
    };
    template <typename R, typename ...Ts>
    struct callable_traits<R(*)(Ts...)>
    {
        using type = R(Ts...);
        enum : size_t { arity = sizeof...(Ts) };
    };
    template <typename R, typename ...Ts>
    struct callable_traits<R(*&)(Ts...)>
    {
        using type = R(Ts...);
        enum : size_t { arity = sizeof...(Ts) };
    };
    template <typename R, typename ...Ts>
    struct callable_traits<R(&)(Ts...)>
    {
        using type = R(Ts...);
        enum : size_t { arity = sizeof...(Ts) };
    };

} // namespace cat
