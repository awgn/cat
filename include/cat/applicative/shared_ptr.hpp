#pragma once

#include <memory>

#include <cat/functor/shared_ptr.hpp>
#include <cat/applicative/applicative.hpp>

namespace cat
{
    // shared_ptr is an applicative instance:

    template <> struct is_applicative<std::shared_ptr> : std::true_type { };

    // shared_ptr instance:
    //

    template <typename Fun, typename A>
    struct ApplicativeInstance<std::shared_ptr, Fun, A> : Applicative<std::shared_ptr>::Class<Fun, A>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        auto pure(A const &elem) const -> std::shared_ptr<A> final
        {
            return std::make_shared<A>(elem);
        }

        auto apply(std::shared_ptr<Fun> const &f, std::shared_ptr<A> const &x) const
                -> std::shared_ptr<B> final
        {
            if (f && x)
                return std::make_shared<B>((*f)(*x));

            return std::shared_ptr<B>{};
        }
    };

} // namespace cat

