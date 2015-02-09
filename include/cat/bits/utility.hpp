#pragma once

#include <utility>

namespace cat
{
    struct id
    {
        template <typename T>
        auto operator()(T &&x) const
        {
            return std::forward<T>(x);
        }
    };

} // namespace cat
