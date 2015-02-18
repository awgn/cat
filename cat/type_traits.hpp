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

#include <iostream>
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

    namespace details
    {
        CAT_CLASS_HAS_MEMBER(value_type);
        CAT_CLASS_HAS_MEMBER(key_type);
        CAT_CLASS_HAS_MEMBER(mapped_type);
        CAT_CLASS_HAS_MEMBER(container_type);

        CAT_CLASS_HAS_MEMBER(pointer);
        CAT_CLASS_HAS_MEMBER(const_pointer);
        CAT_CLASS_HAS_MEMBER(reference);
        CAT_CLASS_HAS_MEMBER(const_reference);
        CAT_CLASS_HAS_MEMBER(iterator);
        CAT_CLASS_HAS_MEMBER(const_iterator);
        CAT_CLASS_HAS_MEMBER(reverse_iterator);
        CAT_CLASS_HAS_MEMBER(const_reverse_iterator);
        CAT_CLASS_HAS_MEMBER(size_type);
        CAT_CLASS_HAS_MEMBER(difference_type);

    } // namespace details


    //////////////////////////////////////////////////////////////////////////////////
    //
    // generic type traits...
    //

    CAT_CLASS_HAS_TYPEDEF(function_type);
    CAT_CLASS_HAS_MEMBER(arity_value);


    template <typename T>
    struct has_value_type : std::integral_constant<bool, details::has_value_type<T>::value>
    {};

    template <typename t>
    struct has_key_type : std::integral_constant<bool, details::has_key_type<t>::value>
    {};

    template <typename t>
    struct has_mapped_type : std::integral_constant<bool, details::has_mapped_type<t>::value>
    {};

    template <typename t>
    struct has_container_type : std::integral_constant<bool, details::has_container_type<t>::value>
    {};

    template <typename T>
    struct has_pointer : std::integral_constant<bool, details::has_pointer<T>::value>
    {};

    template <typename T>
    struct has_const_pointer : std::integral_constant<bool, details::has_const_pointer<T>::value>
    {};

    template <typename T>
    struct has_reference : std::integral_constant<bool, details::has_reference<T>::value>
    {};

    template <typename T>
    struct has_const_reference : std::integral_constant<bool, details::has_const_reference<T>::value>
    {};

    template <typename T>
    struct has_iterator : std::integral_constant<bool, details::has_iterator<T>::value>
    {};

    template <typename T>
    struct has_const_iterator : std::integral_constant<bool, details::has_const_iterator<T>::value>
    {};

    template <typename T>
    struct has_reverse_iterator : std::integral_constant<bool, details::has_reverse_iterator<T>::value>
    {};

    template <typename T>
    struct has_const_reverse_iterator : std::integral_constant<bool, details::has_const_reverse_iterator<T>::value>
    {};

    template <typename T>
    struct has_size_type : std::integral_constant<bool, details::has_size_type<T>::value>
    {};

    template <typename T>
    struct has_difference_type : std::integral_constant<bool, details::has_difference_type<T>::value>
    {};


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_container
    //

    template <typename T>
    struct is_container : std::integral_constant<bool, details::has_value_type<T>::value &&
                                                       details::has_reference<T>::value &&
                                                       details::has_const_reference<T>::value &&
                                                       details::has_iterator<T>::value &&
                                                       details::has_const_iterator<T>::value &&
                                                       details::has_pointer<T>::value &&
                                                       details::has_const_pointer<T>::value &&
                                                       details::has_size_type<T>::value &&
                                                       details::has_difference_type<T>::value>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_associative_container
    //

    template <typename T>
    struct is_associative_container : std::integral_constant<bool, is_container<T>::value &&
                                                                   details::has_key_type<T>::value &&
                                                                   details::has_mapped_type<T>::value>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // not_type
    //

    template <typename Trait, bool V = Trait::value>
    struct not_type : std::false_type {};

    template <typename Trait>
    struct not_type<Trait, false> : std::true_type {};


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_tuple
    //

    template <typename T>
    struct is_tuple : std::integral_constant<bool, false>
    { };

    template <typename ...Ti>
    struct is_tuple<std::tuple<Ti...>> : std::integral_constant<bool, true>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_pair
    //

    template <typename T>
    struct is_pair : std::integral_constant<bool, false>
    { };

    template <typename T, typename U>
    struct is_pair<std::pair<T,U>> : std::integral_constant<bool, true>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // has_insertion_operator: (<<)
    //

    template <typename T>
    class __has_insertion_operator
    {
        template <typename C> static void test(std::remove_reference_t<decltype((std::cout << std::declval<C>())) > *) noexcept;
        template <typename C> static void test(...) noexcept(false);
    public:
        enum { value = noexcept(test<T>(0)) };
    };

    template <typename T>
    struct has_insertion_operator
        : std::integral_constant<bool, __has_insertion_operator<T>::value>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // has_extraction_operator: (>>)
    //

    template <typename T>
    class __has_extraction_operator
    {
        template <typename C> static void test(std::remove_reference_t<decltype((std::cin >> std::declval<C &>())) > *) noexcept;
        template <typename C> static void test(...) noexcept(false);
    public:
        enum { value = noexcept(test<T>(0)) };
    };

    template <typename T>
    struct has_extraction_operator
        : std::integral_constant<bool, __has_extraction_operator<T>::value>
    {};


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_copy_constructing:
    //

    template <typename T, typename ...U>
    struct is_copy_constructing : std::false_type {};

    template <typename T, typename U>
    struct is_copy_constructing<T,U> : std::is_same<typename std::decay<T>::type, typename std::decay<U>::type> {};

    // is_not_copy_constructing:
    // trait useful to disable universal constructor to enable copy constructor:
    //
    //  template <typename ...Ts, typename = typename std::enable_if<is_not_copy_constructing<ThisClass, Ts...>::value>::type>
    //  ThisClass(Ts&& ...args)
    //

    template <typename T, typename ...U>
    struct is_not_copy_constructing : not_type<is_copy_constructing<T,U...>> {};


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
    template <typename T, typename To>
    struct rebind<T, To, std::enable_if_t<is_default_deleter<T>::value>>
    {
        using type = std::default_delete<To>;
    };
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

    template <typename F, size_t N>
    struct _partial_function
    {
        using type = F;
    };

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

    template <typename F>
    struct _callable_traits : _callable_traits<decltype(&F::operator())> { };

    template <typename F, typename R, typename ...Ts>
    struct _callable_traits<R(F::*)(Ts...) const>
    {
        using function_type = R(Ts...);
        enum : size_t { arity_value = sizeof...(Ts) };
    };
    template <typename F, typename R, typename ...Ts>
    struct _callable_traits<R(F::*)(Ts...)>
    {
        using function_type = R(Ts...);
        enum : size_t { arity_value = sizeof...(Ts) };
    };
    template <typename R, typename ...Ts>
    struct _callable_traits<R(Ts...)>
    {
        using function_type = R(Ts...);
        enum : size_t { arity_value = sizeof...(Ts) };
    };
    template <typename R, typename ...Ts>
    struct _callable_traits<R(*)(Ts...)>
    {
        using function_type = R(Ts...);
        enum : size_t { arity_value = sizeof...(Ts) };
    };
    template <typename R, typename ...Ts>
    struct _callable_traits<R(*&)(Ts...)>
    {
        using function_type = R(Ts...);
        enum : size_t { arity_value = sizeof...(Ts) };
    };
    template <typename R, typename ...Ts>
    struct _callable_traits<R(&)(Ts...)>
    {
        using function_type = R(Ts...);
        enum : size_t { arity_value = sizeof...(Ts) };
    };

    template <typename F> struct _result_type;
    template <typename R, typename ...Ts>
    struct _result_type<R(Ts...)>
    {
        using type = R;
    };


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
        using type = typename _result_type<typename function_type<F>::type>::type;
    };

    template <typename F>
    struct arity
    {
        using G = typename std::decay<F>::type;
        enum { value = std::conditional< has_arity_value<G>::value, G, _callable_traits<G> >::type::arity_value };
    };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // has_call_operator
    //

    template <typename T>
    class has_call_operator
    {
        template <class C> static void check(decltype(&C::operator()) *) noexcept;
        template <class C> static void check(...) noexcept(false);
    public:
        enum { value = noexcept(check<T>(nullptr)) };
    };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_function (std)
    //

    template <typename F> struct is_function : std::false_type { };

    template <typename R, typename ...Ts>
    struct is_function<std::function<R(Ts...)>> : std::false_type { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_callable_with, is_callable_as....
    //

    template <typename F, typename ...Ts>
    class is_callable_with
    {
        template <class C> static void check(std::decay_t<decltype(std::declval<C>()(std::declval<Ts>()...))> *) noexcept;
        template <class C> static void check(...) noexcept(false);
    public:
        enum { value = noexcept(check<F>(nullptr)) };
    };

    template <typename F> struct is_callable_as;
    template <typename F, typename ...Ts>
    struct is_callable_as<F(Ts...)> : is_callable_with<F, Ts...> { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_callable....
    //

    template <typename F>
    struct _is_callable : std::integral_constant<bool,
                            std::is_function<F>::value  ||
                            is_function<F>::value       ||
                            has_function_type<F>::value ||
                            has_call_operator<F>::value>
    { };

    template <typename F>
    struct is_callable : _is_callable<std::remove_pointer_t<std::decay_t<F>>> { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // inner_type....
    //

    template <typename C, size_t Idx = 0> struct inner_type;

    template <template <typename ...> class F, typename T, typename ...Ts>
    struct inner_type<F<T, Ts...>, 0>
    {
        using type = T;
    };
    template <template <typename ...> class F, typename T, typename T1, typename ...Ts>
    struct inner_type<F<T, T1, Ts...>, 1>
    {
        using type = T1;
    };
    template <template <typename ...> class F, typename T, typename T1, typename T2, typename ...Ts>
    struct inner_type<F<T, T1, T2, Ts...>, 2>
    {
        using type = T2;
    };

    template <typename T>
    using inner_type_t = typename inner_type<T>::type;


} // namespace cat
