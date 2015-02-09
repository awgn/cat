#pragma once

#include <memory>

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

}
