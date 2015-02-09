#pragma once

#include <utility>

namespace cat
{
    struct Id
    {
        template <typename T>
        auto operator()(T &&x) const
        {
            return std::forward<T>(x);
        }
    };

} // namespace cat

