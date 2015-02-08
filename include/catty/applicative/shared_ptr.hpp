#pragma once

#include <memory>

#include <catty/functor/shared_ptr.hpp>
#include <catty/applicative/applicative.hpp>

namespace catty
{
    // shared_ptr instance:
    //

    template <typename A>
    auto pure(A const &elem, tag<std::shared_ptr>)
    {
        return std::make_shared<A>(elem);
    }

    template <typename Fun, typename A>
    auto apply(std::shared_ptr<Fun> const &f, std::shared_ptr<A> const &x)
    {
        using type = decltype(std::declval<Fun>()(std::declval<A>()));

        if (f && x) {
            return std::make_shared<type>((*f)(*x));
        }

        return std::shared_ptr<type>();
    }

    template <>
    struct typeclass_instance2<Applicative, std::shared_ptr>
    {
        using type =
            typeclass
            <
                OVERLOADED_FUNCTION(pure,  a_ const&, std::shared_ptr<a_> ),
                OVERLOADED_FUNCTION(apply, std::shared_ptr<F_<a_,b_>> const &, std::shared_ptr<a_> const&, std::shared_ptr<b_> )
            >;
    };

} // namespace catty

