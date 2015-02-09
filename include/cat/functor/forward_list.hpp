#pragma once

#include <forward_list>

#include <cat/functor/functor.hpp>


namespace cat
{
    // forward_list is a functor:

    template <> struct is_functor<std::forward_list> : std::true_type { };

    // forward_list instance:

    template <typename Fun, typename A, typename Alloc>
    struct FunctorInstance<std::forward_list, Fun, A, Alloc> : Functor<std::forward_list>::Class<Fun, A, Alloc>
    {
        using B = decltype(std::declval<Fun>()(std::declval<A>()));

        auto fmap(Fun f, std::forward_list<A, Alloc> const &xs) const
                -> std::forward_list<B, rebind_t<Alloc, B> > final
        {
            std::forward_list<B, rebind_t<Alloc, B> > out;

            for(auto & x : xs)
                out.push_front(f(x));

            out.reverse();
            return out;
        }
    };

} // namespace cat

