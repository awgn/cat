#pragma once

#include <memory>

#include <cat/functor/unique_ptr.hpp>
#include <cat/applicative/applicative.hpp>

namespace cat
{
    // unique_ptr is an applicative instance:
    //

    template <> struct is_applicative<std::unique_ptr> : std::true_type { };

    // unique_ptr instance:
    //

    template <typename Fun, typename A, typename ...Ts>
    struct ApplicativeInstance<std::unique_ptr, Fun, A, Ts...> : Applicative<std::unique_ptr>::Class<Fun, A, Ts...>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        std::unique_ptr<A>
        pure(A const &elem) final
        {
            return std::make_unique<A>(elem);
        }

        std::unique_ptr<B>
        apply(std::unique_ptr<Fun> const &f, std::unique_ptr<A, Ts...> const &x) final
        {
            if (f && x)
                return std::make_unique<B>((*f)(*x));

            return std::unique_ptr<B>{};
        }
    };

} // namespace cat

