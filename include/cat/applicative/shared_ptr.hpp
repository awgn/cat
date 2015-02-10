#pragma once

#include <memory>

#include <cat/functor/shared_ptr.hpp>
#include <cat/applicative/applicative.hpp>

namespace cat
{
    // shared_ptr is an applicative instance:
    //

    template <> struct is_applicative<std::shared_ptr> : std::true_type { };

    // shared_ptr instance:
    //

    template <typename Fun, typename A, typename ...Ts>
    struct ApplicativeInstance<std::shared_ptr, Fun, A, Ts...> : Applicative<std::shared_ptr>::Class<Fun, A, Ts...>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        std::shared_ptr<A>
        pure(A const &elem) final
        {
            return std::make_shared<A>(elem);
        }

        std::shared_ptr<B>
        apply(std::shared_ptr<Fun> const &f, std::shared_ptr<A> const &x) final
        {
            if (f && x)
                return std::make_shared<B>((*f)(*x));

            return std::shared_ptr<B>{};
        }
    };

} // namespace cat

