#pragma once

#include <memory>

#include <cat/functor/functor.hpp>

namespace cat
{
    // unique_ptr is a functor:
    //

    template <> struct is_functor<std::unique_ptr> : std::true_type { };

    // unique_ptr instance:

    template <typename Fun, typename A, typename Deleter>
    struct FunctorInstance<std::unique_ptr, Fun, A, Deleter> : Functor<std::unique_ptr>::Class<Fun, A>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        auto fmap(Fun f, std::unique_ptr<A, Deleter> const &xs) const
                -> std::unique_ptr<B> final
        {
            using type = decltype(f(*xs));

            if (xs)
                return std::make_unique<type>(f(*xs));

            return std::unique_ptr<type>();
        }
    };

} // namespace cat

