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

#include <cat/pretty/pretty.hpp>
#include <cat/existential.hpp>
#include <any>

namespace cat
{
    //
    // forall_1<Pretty> class (type erasure)
    //

    template <>
    struct forall_1<Pretty>
    {
    private:

        template <typename T>
        struct prettyable : forall_base
        {
            template <typename V>
            prettyable(V const &v)
            : value(v)
            {}

            T value;

            virtual std::any run_forall() override
            {
                return cat::pretty(value);
            }
        };

    public:

        template <typename T>
        forall_1(const T &v)
        : value(std::make_shared<prettyable<T>>(v))
        { }

        std::shared_ptr<forall_base> value;
    };

    //
    // Pretty instance...
    //

    template <>
    struct PrettyInstance<forall_1<Pretty>> final : Pretty<forall_1<Pretty>>
    {
        std::string
        pretty(forall_1<Pretty> const &e) override
        {
            return std::any_cast<std::string>(e.value->run_forall());
        }
    };
}
