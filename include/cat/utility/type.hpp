#pragma once

#include <string>
#include <type_traits>

#include <cxxabi.h>

namespace cat
{
    //
    // demangle type name
    //

    inline std::string
    demangle(const char * name)
    {
        int status;
        std::unique_ptr<char, void(*)(void *)> ret(abi::__cxa_demangle(name,0,0, &status), ::free);
        if (status < 0)
            return std::string(1,'?');

        return std::string(ret.get());
    }

    //
    // get the demangled name of a type
    //

    template <typename Tp>
    std::string type_name()
    {
        auto name = demangle(typeid(Tp).name());
        if (std::is_lvalue_reference<Tp>::value)
            name += "&";
        else if (std::is_rvalue_reference<Tp>::value)
            name += "&&";
        return name;
    }


    //
    // get the demangled name of an expression
    //

    template <typename Tp>
    std::string
    type_of(Tp && arg)
    {
        auto name = demangle(typeid(Tp).name());

        if (std::is_const<
            typename std::remove_reference<Tp>::type>::value)
            name.append(" const");
        if (std::is_volatile<
            typename std::remove_reference<Tp>::type>::value)
            name.append(" volatile");

        if (std::is_reference<Tp>::value)
            name.append("&");
        else
            name.append("&&");

        return name;
    }

} // namespace cat
