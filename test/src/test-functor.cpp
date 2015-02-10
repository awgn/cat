#include <cat/functor.hpp>

#include <yats.hpp>

using namespace yats;
using namespace cat;

//
// user-defined functor:
//

namespace cat
{
    namespace my
    {
        template <typename T>
        struct functor
        {
            T arg;
        };

        template <typename T>
        inline bool operator==(functor<T> const &lhs, functor<T> const &rhs)
        {
            return lhs.arg == rhs.arg;
        }

    }

    template <typename Fun, typename A>
    struct FunctorInstance<my::functor, Fun, A> : Functor<my::functor>::Class<Fun, A>
    {
        auto fmap(Fun fun, my::functor<A> const &x)
            -> my::functor<decltype(fun(std::declval<A>()))> final
        {
            my::functor< decltype(fun(x.arg)) > out;
            out.arg = fun(x.arg);
            return out;
        }
    };
}


// Tests:
//

Context(functor)
{
    template <template <typename...> class F, typename ...Ts>
    void functor_constraint(F<Ts...> const &)
    {
        static_assert(is_functor<F>::value, "F: not a functor!");
    }


    Test(my_functor)
    {
        my::functor<std::string> x{ "hello" };

        auto y = fmap([](std::string const &x) {
                            return x.size();
                      }, x);

        Assert (y == my::functor<size_t>{ 5 });
    }


    Test(functor_vector)
    {
        std::vector<std::string> v { "one", "two", "three" };

        auto r = fmap([](const std::string &s) -> size_t { return s.size(); }, v);

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


    Test(functor_map)
    {
        std::map<int, std::string> m { {1,"one"}, {2,"two"}, {3,"three"} };

        auto r = fmap([](const std::string &s) { return s.size(); }, m);

        Assert(r, is_equal_to(std::map<int,size_t>{{1,3}, {2,3}, {3,5}}));
    }


    Test(functor_unordered_map)
    {
        std::unordered_map<int, std::string> m { {1,"one"}, {2,"two"}, {3,"three"} };

        auto r = fmap([](const std::string &s) { return s.size(); }, m);

        Assert(r, is_equal_to(std::unordered_map<int,size_t>{{1,3}, {2,3}, {3,5}}));
    }


    Test(functor_constraint)
    {
        functor_constraint( std::vector<std::string>{} );
        functor_constraint( std::deque<std::string>{} );
        functor_constraint( std::list<std::string>  { "one", "two", "three" });
        functor_constraint( std::forward_list<std::string>  { "one", "two", "three" });
        functor_constraint( std::make_shared<std::string>( "one" ));
        functor_constraint( std::make_unique<std::string>( "one" ));
        functor_constraint( std::experimental::make_optional<std::string>( "one" ));

        functor_constraint( std::map<std::string, int>{} );
        functor_constraint( std::unordered_map<std::string, int>{} );
    }

}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

