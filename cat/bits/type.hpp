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
#include <cstdlib>
#include <string>
#include <memory>

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
    type_of(Tp &&)
    {
        auto name = demangle(typeid(Tp).name());

        if (std::is_const<std::remove_reference_t<Tp>>::value)
            name.append(" const");
        if (std::is_volatile<std::remove_reference_t<Tp>>::value)
            name.append(" volatile");

        if (std::is_reference<Tp>::value)
            name.append("&");
        else
            name.append("&&");

        return name;
    }

    //
    // undefined
    //

    template <typename ...Ts> struct undefined;


} // namespace cat
