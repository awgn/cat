#pragma once

#include <memory>

#include <cat/functor/functor.hpp>

namespace cat
{
    // shared_ptr is functor:

    template <> struct is_functor<std::shared_ptr> : std::true_type { };

    // shared_ptr instance:

    template <typename Fun, typename A>
    struct FunctorInstance<std::shared_ptr, Fun, A> : Functor<std::shared_ptr>::Class<Fun, A>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        auto fmap(Fun f, std::shared_ptr<A> const &xs) -> std::shared_ptr<B> final
        {
            using type = decltype(f(*xs));

            if (xs)
                return std::make_shared<type>(f(*xs));

            return std::shared_ptr<type>();
        }
    };


} // namespace cat

