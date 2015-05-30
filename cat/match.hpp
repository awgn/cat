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

#include <cat/__config.hpp>
#include <cat/type_traits.hpp>

//
// Inspired to http://stackoverflow.com/questions/18432260/lambda-functions-as-base-classes
//

namespace cat
{
    namespace details
    {
        template <typename ...Fs> struct Overload_;

        template <typename F, typename ...Fs>
        struct Overload_<F, Fs...> : F, Overload_<Fs...>
        {
            template <typename G, typename ...Gs>
            constexpr Overload_(G && g, Gs && ... gs)
            : F(std::forward<G>(g)) , Overload_<Gs...>(std::forward<Gs>(gs)...)
            {}

            using F::operator();
            using Overload_<Fs...>::operator();
        };

        template <typename F>
        struct Overload_<F> : F
        {
            template <typename G>
            constexpr Overload_(G && g)
            : F (std::forward<G>(g))
            {}

            using F::operator();
        };


        template <typename T>
        struct Match_
        {
            constexpr Match_(T && expr)
            : expr_(std::forward<T>(expr))
            { }

            template <typename ...Fs>
            auto match(Fs &&...fs)
            {
                return Overload_<Fs...>(std::forward<Fs>(fs)...)(std::forward<T>(expr_));
            }

        private:

            T && expr_;
        };
    }

    template <typename T>
    constexpr auto
    case_of(T && expr)
    {
        return details::Match_<T>{std::forward<T>(expr)};
    }
}

