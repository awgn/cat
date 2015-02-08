#pragma once

#include <memory>

#include <catty/functor/unique_ptr.hpp>
#include <catty/applicative/applicative.hpp>

namespace catty
{
    // unique_ptr instance:
    //

    template <typename A>
    auto pure(A const &elem, tag<std::unique_ptr>)
    {
        return std::make_shared<A>(elem);
    }

    template <typename Fun, typename A>
    auto apply(std::unique_ptr<Fun> const &f, std::unique_ptr<A> const &x)
    {
        using type = decltype(std::declval<Fun>()(std::declval<A>()));

        if (f && x) {
            return std::make_unique<type>((*f)(*x));
        }

        return std::unique_ptr<type>();
    }

    template <>
    struct typeclass_instance2<Applicative, std::unique_ptr>
    {
        using type =
            typeclass
            <
                OVERLOADED_FUNCTION(pure,  a_ const&, std::unique_ptr<a_> ),
                OVERLOADED_FUNCTION(apply, std::unique_ptr<F_<a_,b_>> const &, std::unique_ptr<a_> const&, std::unique_ptr<b_> )
            >;
    };

} // namespace catty

