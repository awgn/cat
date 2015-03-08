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


#define CAT_CLASS_HAS_TYPEDEF(typedef_) \
    template <typename T> \
    class has_ ## typedef_ \
    { \
        template <class C> static void check(typename std::decay<typename C::typedef_>::type *) noexcept; \
        template <class C> static void check(...) noexcept(false); \
    public: \
    \
        enum { value = noexcept(check<T>(nullptr)) }; \
    };

#define CAT_CLASS_HAS_MEMBER(member) \
    template <typename T> \
    class has_ ## member \
    { \
        template <class C> static void check(typename std::decay<typename C::member>::type *) noexcept; \
        template <class C> static void check(...) noexcept(false); \
    public: \
    \
        enum { value = noexcept(check<T>(nullptr)) }; \
    };


namespace cat
{

    //////////////////////////////////////////////////////////////////////////////////
    //
    // the identity meta-function
    //

    template <typename T>
    struct identity_type
    {
        using type = T;
    };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // boolean type
    //

    template <bool value>
    using bool_type = std::integral_constant<bool, value>;

    template <typename Trait, bool V = Trait::value> struct not_type;

    template <typename Trait>
    struct not_type<Trait, false> : std::true_type { };

    template <typename Trait>
    struct not_type<Trait, true> : std::false_type { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // generic type traits...
    //

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

        CAT_CLASS_HAS_TYPEDEF(function_type);
        CAT_CLASS_HAS_TYPEDEF(return_type);
        CAT_CLASS_HAS_MEMBER(arity_value);
    }


    template <typename T>
    struct has_value_type
        : bool_type<details::has_value_type<T>::value>
    { };

    template <typename t>
    struct has_key_type
        : bool_type<details::has_key_type<t>::value>
    { };

    template <typename t>
    struct has_mapped_type
        : bool_type<details::has_mapped_type<t>::value>
    { };

    template <typename t>
    struct has_container_type
        : bool_type<details::has_container_type<t>::value>
    { };

    template <typename T>
    struct has_pointer
        : bool_type<details::has_pointer<T>::value>
    { };

    template <typename T>
    struct has_const_pointer
        : bool_type<details::has_const_pointer<T>::value>
    { };

    template <typename T>
    struct has_reference
        : bool_type<details::has_reference<T>::value>
    { };

    template <typename T>
    struct has_const_reference
        : bool_type<details::has_const_reference<T>::value>
    { };

    template <typename T>
    struct has_iterator
        : bool_type<details::has_iterator<T>::value>
    { };

    template <typename T>
    struct has_const_iterator
        : bool_type<details::has_const_iterator<T>::value>
    { };

    template <typename T>
    struct has_reverse_iterator
        : bool_type<details::has_reverse_iterator<T>::value>
    { };

    template <typename T>
    struct has_const_reverse_iterator
        : bool_type<details::has_const_reverse_iterator<T>::value>
    { };

    template <typename T>
    struct has_size_type
        : bool_type<details::has_size_type<T>::value>
    { };

    template <typename T>
    struct has_difference_type
        : bool_type<details::has_difference_type<T>::value>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_container
    //

    template <typename T>
    struct is_container
        : bool_type<details::has_value_type<T>::value &&
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
    struct is_associative_container
        : bool_type<is_container<T>::value &&
                    details::has_key_type<T>::value &&
                    details::has_mapped_type<T>::value>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_tuple
    //

    template <typename T>
    struct is_tuple : bool_type<false>
    { };

    template <typename ...Ti>
    struct is_tuple<std::tuple<Ti...>> : bool_type<true>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_pair
    //

    template <typename T>
    struct is_pair : bool_type<false>
    { };

    template <typename T, typename U>
    struct is_pair<std::pair<T,U>> : bool_type<true>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // has_insertion_operator: (<<)
    //

    namespace details
    {
        template <typename T>
        class has_insertion_operator
        {
            template <typename C> static void test(std::remove_reference_t<decltype((std::cout << std::declval<C>())) > *) noexcept;
            template <typename C> static void test(...) noexcept(false);
        public:
            enum { value = noexcept(test<T>(nullptr)) };
        };

    }

    template <typename T>
    struct has_insertion_operator
        : bool_type<details::has_insertion_operator<T>::value>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // has_extraction_operator: (>>)
    //

    namespace details
    {
        template <typename T>
        class has_extraction_operator
        {
            template <typename C> static void test(std::remove_reference_t<decltype((std::cin >> std::declval<C &>())) > *) noexcept;
            template <typename C> static void test(...) noexcept(false);
        public:
            enum { value = noexcept(test<T>(nullptr)) };
        };

    }

    template <typename T>
    struct has_extraction_operator
        : bool_type<details::has_extraction_operator<T>::value>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_copy_constructing:
    //

    template <typename T, typename ...U>
    struct is_copy_constructing
        : std::false_type
    { };

    template <typename T, typename U>
    struct is_copy_constructing<T,U>
        : std::is_same<typename std::decay<T>::type, typename std::decay<U>::type>
    { };

    // is_not_copy_constructing:
    // trait useful to disable universal constructor to enable copy constructor:
    //
    //  template <typename ...Ts, std::enable_if_t<is_not_copy_constructing<ThisClass, Ts...>::value> * = nullptr>
    //  ThisClass(Ts&& ...args)
    //

    template <typename T, typename ...U>
    struct is_not_copy_constructing
        : not_type<is_copy_constructing<T,U...>>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // function_type
    //

    namespace details
    {
        template <typename F>
        struct function_type_ : function_type_<decltype(&F::operator())> { };

        template <typename F, typename R, typename ...Ts>
        struct function_type_<R(F::*)(Ts...) const>
        {
            using function_type = R(Ts...);
        };

        template <typename F, typename R, typename ...Ts>
        struct function_type_<R(F::*)(Ts...)>
        {
            using function_type = R(Ts...);
        };
        template <typename R, typename ...Ts>
        struct function_type_<R(Ts...)>
        {
            using function_type = R(Ts...);
        };
        template <typename R, typename ...Ts>
        struct function_type_<R(*)(Ts...)>
        {
            using function_type = R(Ts...);
        };
        template <typename R, typename ...Ts>
        struct function_type_<R(*&)(Ts...)>
        {
            using function_type = R(Ts...);
        };
        template <typename R, typename ...Ts>
        struct function_type_<R(&)(Ts...)>
        {
            using function_type = R(Ts...);
        };

        template <typename F> struct arity;
        template <typename R, typename ...Ts>
        struct arity<R(Ts...)>
        {
            enum { value = sizeof...(Ts) };
        };

        template <typename F> struct return_type;
        template <typename R, typename ...Ts>
        struct return_type<R(Ts...)>
        {
            using type = R;
        };
    }

    template <typename F>
    struct function_type
    {
        using _F = std::decay_t<F>;
        using type = typename std::conditional_t<details::has_function_type<F>::value, _F, details::function_type_<_F> >::function_type;
    };

    template <typename F>
    using function_type_t = typename function_type<F>::type;

    //////////////////////////////////////////////////////////////////////////////////
    //
    // function arity
    //

    template <typename F>
    struct arity : details::arity< function_type_t<F> >
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // return type
    //

    template <typename F>
    struct return_type : details::return_type< function_type_t<F> >
    { };

    template <typename F>
    using return_type_t = typename return_type<F>::type;


    //////////////////////////////////////////////////////////////////////////////////
    //
    // has_call_operator
    //

    namespace details
    {
        template <typename T>
        class has_call_operator
        {
            template <class C> static void check(decltype(&C::operator()) *) noexcept;
            template <class C> static void check(...) noexcept(false);
        public:
            enum { value = noexcept(check<T>(nullptr)) };
        };

    }

    template <typename T>
    struct has_call_operator
        : bool_type< details::has_call_operator<T>::value >
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_std_function (std)
    //

    template <typename F> struct is_std_function : std::false_type { };

    template <typename R, typename ...Ts>
    struct is_std_function<std::function<R(Ts...)>> : std::true_type { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_callable_with
    //

    namespace details
    {
        template <typename F, typename ...Ts>
        class is_callable_with
        {
            template <class C> static void check(std::decay_t<decltype(std::declval<C>()(std::declval<Ts>()...))> *) noexcept;
            template <class C> static void check(...) noexcept(false);
        public:
            enum { value = noexcept(check<F>(nullptr)) };
        };
    }

    template <typename F, typename ... Ts>
    struct is_callable_with
        : bool_type<details::is_callable_with<F, Ts...>::value>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_callable_as
    //

    template <typename F> struct is_callable_as;
    template <typename F, typename ... Ts>
    struct is_callable_as<F(Ts...)> : is_callable_with<F, Ts...>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // is_callable....
    //

    namespace details
    {
        template <typename F>
        struct is_callable
        {
            enum { value = std::is_function<F>::value  ||
                           is_std_function<F>::value   ||
                           has_function_type<F>::value ||
                           has_call_operator<F>::value };
        };
    }

    template <typename F>
    struct is_callable
        : bool_type<details::is_callable<std::remove_pointer_t<std::decay_t<F>>>::value>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // map_result_of: apply functor over inner types and return a new type with
    // each result_of
    //

    template <typename Type, typename ...Fs> struct map_result_of;

    template <template <typename ...> class Functor,
              typename T0, typename F>
    struct map_result_of <Functor<T0>, F>
    {
        using type = Functor< typename std::result_of_t<F(T0)>>;
    };
    template <template <typename ...> class Functor,
              typename T0, typename T1,
              typename F0, typename F1>
    struct map_result_of <Functor<T0, T1>, F0, F1>
    {
        using type = Functor< std::result_of_t<F0(T0)>,
                                       std::result_of_t<F1(T1)>>;
    };
    template <template <typename ...> class Functor,
              typename T0, typename T1, typename T2,
              typename F0, typename F1, typename F2>
    struct map_result_of <Functor<T0, T1, T2>, F0, F1, F2>
    {
        using type = Functor< std::result_of_t<F0(T0)>,
                                        std::result_of_t<F1(T1)>,
                                        std::result_of_t<F2(T2)> >;
    };

    template <typename Type, typename ...Fs>
    using map_result_of_t = typename map_result_of<Type, Fs...>::type;


    //////////////////////////////////////////////////////////////////////////////////
    //
    // type_index
    //

    template <size_t N, typename ...Ts> struct type_index;

    template <typename T, typename ...Ts>
    struct type_index<0, T, Ts...>
    {
        using type = T;
    };

    template <size_t N, typename T, typename ...Ts>
    struct type_index<N, T, Ts...> : type_index<N-1, Ts...>
    { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // arg_type
    //

    template <typename F, size_t N> struct arg_type;

    template <typename R, typename ...Ts, size_t N>
    struct arg_type<R(Ts...), N> : type_index<N, Ts...>
    { };

    template <typename T, size_t N>
    using arg_type_t = typename arg_type<T, N>::type;


    //////////////////////////////////////////////////////////////////////////////////
    //
    // outer_type....
    //

    template <template <typename ...> class > struct template_class;

    template <typename T> struct outer_type;

    template <template <typename ...> class F, typename ...Vs>
    struct outer_type< F<Vs...> >
    {
        using type = template_class<F>;

        template <typename ...Ts>
        struct other
        {
            using type = F<Ts...>;
        };
    };

    template <typename T, size_t N = 0>
    using outer_type_t = typename outer_type<T>::type;


    //////////////////////////////////////////////////////////////////////////////////
    //
    // inner_type....
    //

    template <typename F, size_t N = 0> struct inner_type;

    template <size_t N, template <typename ...> class F, typename ...Ts>
    struct inner_type<F<Ts...>, N> : type_index<N, Ts...>
    { };

    template <typename T, size_t N = 0>
    using inner_type_t = typename inner_type<T, N>::type;


    //////////////////////////////////////////////////////////////////////////////////
    //
    // rebind_type ....
    //

    template <typename F, typename V> struct rebind_type;

    template <template <typename ...> class F, typename V, typename ...Ts>
    struct rebind_type<F<Ts...>, V>
    {
        using type = F<V>;
    };

    template <typename T, typename V>
    using rebind_type_t = typename rebind_type<T, V>::type;


    //////////////////////////////////////////////////////////////////////////////////
    //
    // partial_function_type
    //

    template <typename F, size_t N, typename = void > struct partial_function_type;

    template <typename R, typename T, typename ...Ts, size_t N>
    struct partial_function_type<R(T, Ts...), N, std::enable_if_t<(N != 0)>>
        : partial_function_type<R(Ts...), N-1> { };

    template <typename R, typename ...Ts, size_t N>
    struct partial_function_type<R(Ts...), N, std::enable_if_t<(N == 0)>>
    {
        using type = R(Ts...);
    };

    template <typename F, size_t N>
    using partial_function_type_t = typename partial_function_type<F, N>::type;


    //////////////////////////////////////////////////////////////////////////////////
    //
    // apply a meta-predicate on the outer type (e.g. functor, monad) of the given type
    //

    template < template <template <typename ...> class> class Trait, typename T>
    struct on_outer_type;
    template < template <template <typename ...> class> class Trait, template <typename ...> class Outer, typename ...Ts>
    struct on_outer_type<Trait, Outer<Ts...>> : Trait<Outer> { };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // apply a meta-function over the inner types
    //

    template <template <typename> class Fun, typename Functor>
    struct fmap_type;

    template <template <typename> class Fun, template <typename ...> class Functor, typename ...Ts>
    struct fmap_type<Fun, Functor<Ts...>>
    {
        using type = Functor<typename Fun<Ts>::type...>;
    };


    //////////////////////////////////////////////////////////////////////////////////
    //
    // has_specialization: e.g. has_specialization<ShowInstance, T>
    //

    namespace details
    {
        template <template <typename...> class Class, typename ...Ts>
        class has_specialization
        {
            template <typename ...Cs> static void test(std::integral_constant<size_t, sizeof(Class<Cs...>)> *) noexcept;
            template <typename ...Cs> static void test(...) noexcept(false);
        public:
            enum { value = noexcept(test<Ts...>(nullptr)) };
        };
    }

    template <template <typename...>  class Class, typename ...Ts>
    struct has_specialization:
        bool_type<details::has_specialization<Class, Ts...>::value>
    { };


} // namespace cat
