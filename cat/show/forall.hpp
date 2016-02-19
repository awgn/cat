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

#include <cat/show/show.hpp>
#include <cat/existential.hpp>

#if (__GNUC__ >= 5)

namespace cat
{
    //
    // forall_1<Show> class (type erasure)
    //

    template <>
    struct forall_1<Show>
    {
    private:

        template <typename T>
        struct showable : forall_base
        {
            template <typename V>
            showable(V const &v)
            : value(v)
            {}

            T value;

            virtual std::experimental::any run_forall() override
            {
                return cat::show(value);
            }
        };

    public:

        template <typename T>
        forall_1(const T &v)
        : value(std::make_shared<showable<T>>(v))
        { }

        std::shared_ptr<forall_base> value;
    };

    //
    // Show instance...
    //

    template <>
    struct ShowInstance<forall_1<Show>> final : Show<forall_1<Show>>
    {
        std::string
        show(forall_1<Show> const &e) override
        {
            return std::experimental::any_cast<std::string>(e.value->run_forall());
        }
    };
}

#endif
