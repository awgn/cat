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

    template <typename Fun, typename A>
    struct ApplicativeInstance<std::unique_ptr, Fun, A> : Applicative<std::unique_ptr>::Class<Fun, A>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        auto pure(A const &elem) const -> std::unique_ptr<A> final
        {
            return std::make_unique<A>(elem);
        }

        auto apply(std::unique_ptr<Fun> const &f, std::unique_ptr<A> const &x) const
                -> std::unique_ptr<B> final
        {
            if (f && x)
                return std::make_unique<B>((*f)(*x));

            return std::unique_ptr<B>{};
        }
    };

} // namespace cat

