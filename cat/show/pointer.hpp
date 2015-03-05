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
#include <cat/type_traits.hpp>

#include <sstream>
#include <memory>

namespace cat
{
    //
    // Instances...
    //

    template <typename T>
    struct ShowInstance<T *> final : Show<T *>
    {
        std::string show(T * const &p) override
        {
            std::ostringstream out;
            out << static_cast<const void *>(p);
            return out.str();
        }
    };

    template <typename T>
    struct ShowInstance<std::unique_ptr<T>> final : Show<std::unique_ptr<T>>
    {
        std::string show(std::unique_ptr<T> const &p) override
        {
            std::ostringstream out;
            out << static_cast<const void *>(p.get()) << "_up";
            return out.str();
        }
    };

    template <typename T>
    struct ShowInstance<std::shared_ptr<T>> final : Show<std::shared_ptr<T>>
    {
        std::string show(std::shared_ptr<T> const &p) override
        {
            std::ostringstream out;
            out << static_cast<const void *>(p.get()) << "_sp" << p.use_count();
            return out.str();
        }
    };
}
