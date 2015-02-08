#include <catty/functor.hpp>

#include <yats.hpp>

using namespace yats;
using namespace catty;


// user-defined functor:
//

namespace my
{
    template <typename T>
    struct functor
    {
        T arg;
    };

    template <typename A, typename Fun >
    auto fmap(Fun fun, my::functor<A> const &x)
    {
        functor< decltype(fun(x.arg)) > out;
        out.arg = fun(x.arg);
        return out;
    }
}

// user-defined instance declaration:
//

namespace catty
{
    template <>
    struct typeclass_instance2<Functor, my::functor>
    {
        using type =
            typeclass
            <
                SYMBOL(fmap, decltype(fmap(_f, my::functor<_A>{})))
            >;
    };
}

//
// Tests:
//

Context(functor)
{
    template <template <typename...> class F, typename T>
    void functor_constraint(F<T> const &)
    {
        static_assert(FunctorInstance<F>(), "F: not a functor!");
    }

    Test(functor_vector)
    {
        std::vector<std::string> v { "one", "two", "three" };

        auto r = fmap([](const std::string &s) { return s.size(); }, v);

        Assert(r, is_equal_to(std::vector<size_t>{3, 3, 5}));
    }


    Test(functor_deque)
    {
        std::deque<std::string> v { "one", "two", "three" };

        auto r = fmap([](const std::string &s) { return s.size(); }, v);

        Assert(r, is_equal_to(std::deque<size_t>{3, 3, 5}));
    }


    Test(functor_list)
    {
        std::list<std::string> v { "one", "two", "three" };

        auto r = fmap([](const std::string &s) { return s.size(); }, v);

        Assert(r, is_equal_to(std::list<size_t>{3, 3, 5}));
    }


    Test(functor_forward_list)
    {
        std::forward_list<std::string> v { "one", "two", "three" };

        auto r = fmap([](const std::string &s) { return s.size(); }, v);

        Assert(r, is_equal_to(std::forward_list<size_t>{3, 3, 5}));
    }


    Test(functor_shared_ptr)
    {
        auto x = std::make_shared<std::string>("one");
        auto y = std::shared_ptr<std::string>();

        auto r1 = fmap([](const std::string &s) { return s.size(); }, x);
        auto r2 = fmap([](const std::string &s) { return s.size(); }, y);

        Assert(r1.get() != nullptr);
        Assert(r2.get() == nullptr);

        Assert(*r1, is_equal_to(3));
    }


    Test(functor_unique_ptr)
    {
        auto x = std::make_unique<std::string>("one");
        auto y = std::unique_ptr<std::string>();

        auto r1 = fmap([](const std::string &s) { return s.size(); }, x);
        auto r2 = fmap([](const std::string &s) { return s.size(); }, y);

        Assert(r1.get() != nullptr);
        Assert(r2.get() == nullptr);

        Assert(*r1, is_equal_to(3));
    }


    Test(functor_optional)
    {
        auto x = std::experimental::make_optional<std::string>("one");
        auto y = std::experimental::optional<std::string>();

        auto r1 = fmap([](const std::string &s) { return s.size(); }, x);
        auto r2 = fmap([](const std::string &s) { return s.size(); }, y);

        Assert(static_cast<bool>(r1) == true);
        Assert(static_cast<bool>(r2) == false);

        Assert(r1.value(), is_equal_to(3));
    }


    // Test(functor_map)
    // {
    //     std::map<int, std::string> m { {1,"one"}, {2,"two"}, {3,"three"} };

    //     auto r = fmap([](const std::string &s) { return s.size(); }, m);

    //     Assert(r, is_equal_to(std::map<int,size_t>{{1,3}, {2,3}, {3,5}}));
    // }


    // Test(functor_unordered_map)
    // {
    //     std::unordered_map<int, std::string> m { {1,"one"}, {2,"two"}, {3,"three"} };

    //     auto r = fmap([](const std::string &s) { return s.size(); }, m);

    //     Assert(r, is_equal_to(std::unordered_map<int,size_t>{{1,3}, {2,3}, {3,5}}));
    // }


    Test(functor_constraint)
    {
        functor_constraint( std::vector<std::string>{} );
        functor_constraint( std::list<std::string>  { "one", "two", "three" });
        functor_constraint( std::forward_list<std::string>  { "one", "two", "three" });
        functor_constraint( std::make_shared<std::string>( "one" ));
        functor_constraint( std::make_unique<std::string>( "one" ));
        functor_constraint( std::experimental::make_optional<std::string>( "one" ));

        functor_constraint( my::functor<int>{ 42 });
    }

}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

