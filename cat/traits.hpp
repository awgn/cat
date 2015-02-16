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

#define CAT_CLASS_HAS_TYPEDEF(typedef_) \
    template <typename T> \
    class has_ ## typedef_ \
    { \
        template <class C> static void check(typename std::decay<typename C::typedef_>::type *) noexcept; \
        template <class C> static void check(...) noexcept(false); \
    public: \
    \
        enum { value = noexcept(check<T>(0)) }; \
    };

#define CAT_CLASS_HAS_MEMBER(member) \
    template <typename T> \
    class has_ ## member \
    { \
        template <class C> static void check(typename std::decay<decltype(C::member)>::type *) noexcept; \
        template <class C> static void check(...) noexcept(false); \
    public: \
    \
        enum { value = noexcept(check<T>(0)) }; \
    };


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

        enum { value = noexcept(check<T>(nullptr)) };
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
    // _partial_function
    //

    template <typename F, size_t N> struct _partial_function;

    template <typename R, typename ...Ts>
    struct _partial_function<R(Ts...), 0>
    {
        using type = R(Ts...);
    };
    template <typename R, typename T, typename ...Ts>
    struct _partial_function<R(T, Ts...), 1>
    {
        using type = R(Ts...);
    };
    template <typename R, typename T1, typename T2, typename ...Ts>
    struct _partial_function<R(T1, T2, Ts...), 2>
    {
        using type = R(Ts...);
    };
    template <typename R, typename T1, typename T2, typename T3, typename ...Ts>
    struct _partial_function<R(T1, T2, T3, Ts...), 3>
    {
        using type = R(Ts...);
    };
    template <typename R, typename T1, typename T2, typename T3, typename T4, typename ...Ts>
    struct _partial_function<R(T1, T2, T3, T4, Ts...), 4>
    {
        using type = R(Ts...);
    };
    template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename ...Ts>
    struct _partial_function<R(T1, T2, T3, T4, T5, Ts...), 5>
    {
        using type = R(Ts...);
    };
    template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename ...Ts>
    struct _partial_function<R(T1, T2, T3, T4, T5, T6, Ts...), 6>
    {
        using type = R(Ts...);
    };
    template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename ...Ts>
    struct _partial_function<R(T1, T2, T3, T4, T5, T6, T7, Ts...), 7>
    {
        using type = R(Ts...);
    };
    template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename ...Ts>
    struct _partial_function<R(T1, T2, T3, T4, T5, T6, T7, T8, Ts...), 8>
    {
        using type = R(Ts...);
    };
    template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename ...Ts>
    struct _partial_function<R(T1, T2, T3, T4, T5, T6, T7, T8, T9, Ts...), 9>
    {
        using type = R(Ts...);
    };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // _callable_traits: function_type, return_type, arity_value....
    //

    template <typename C, typename ...Ts> struct _Callable;

    template <typename F>
    struct _callable_traits : _callable_traits<decltype(&F::operator())> { };

    template <typename F, typename R, typename ...Ts>
    struct _callable_traits<R(F::*)(Ts...) const>
    {
        using function_type = R(Ts...);
        using return_type = R;
        enum : size_t { arity_value = sizeof...(Ts) };
    };
    template <typename F, typename R, typename ...Ts>
    struct _callable_traits<R(F::*)(Ts...)>
    {
        using function_type = R(Ts...);
        using return_type = R;
        enum : size_t { arity_value = sizeof...(Ts) };
    };
    template <typename R, typename ...Ts>
    struct _callable_traits<R(Ts...)>
    {
        using function_type = R(Ts...);
        using return_type = R;
        enum : size_t { arity_value = sizeof...(Ts) };
    };
    template <typename R, typename ...Ts>
    struct _callable_traits<R(*)(Ts...)>
    {
        using function_type = R(Ts...);
        using return_type = R;
        enum : size_t { arity_value = sizeof...(Ts) };
    };
    template <typename R, typename ...Ts>
    struct _callable_traits<R(*&)(Ts...)>
    {
        using function_type = R(Ts...);
        using return_type = R;
        enum : size_t { arity_value = sizeof...(Ts) };
    };
    template <typename R, typename ...Ts>
    struct _callable_traits<R(&)(Ts...)>
    {
        using function_type = R(Ts...);
        using return_type = R;
        enum : size_t { arity_value = sizeof...(Ts) };
    };


    CAT_CLASS_HAS_TYPEDEF(function_type);
    CAT_CLASS_HAS_TYPEDEF(return_type);
    CAT_CLASS_HAS_MEMBER(arity_value);


    template <typename F>
    struct function_type
    {
        using G = typename std::decay<F>::type;
        using type = typename std::conditional< has_function_type<G>::value, G, _callable_traits<G> >::type::function_type;
    };

    template <typename F>
    struct return_type
    {
        using G = typename std::decay<F>::type;
        using type = typename std::conditional< has_return_type<G>::value, G, _callable_traits<G> >::type::return_type;
    };

    template <typename F>
    struct arity
    {
        using G = typename std::decay<F>::type;
        enum { value = std::conditional< has_arity_value<G>::value, G, _callable_traits<G> >::type::arity_value };
    };

} // namespace cat
