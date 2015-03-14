#include <cat/applicative.hpp>
#include <cat/functional.hpp>
#include <cat/bits/type.hpp>

#include "yats.hpp"

using namespace yats;
using namespace cat;


// user-defined applicative:
//

namespace my
{
    template <typename T>
    struct applicative
    {
        T arg;
    };

    template <typename A, typename Fun >
    auto fmap(Fun fun, my::applicative<A> const &x)
    {
        applicative< decltype(fun(x.arg)) > out;
        out.arg = fun(x.arg);
        return out;
    }

    template <typename A>
    auto pure(A const &elem)
    {
        return my::applicative<A>{ elem };
    }


    template <typename Fun, typename A>
    auto apply(my::applicative<Fun> const &fs, my::applicative<A> const &xs)
    {
        applicative< decltype(fs.arg(xs.arg)) > out;
        out.arg = fs.arg(xs.arg);
        return out;
    }

}


namespace cat
{
    template <> struct is_applicative<my::applicative> : std::true_type { };
}

// Tests:
//

Context(applicative)
{

    template <template <typename...> class F, typename ...Ts>
    void applicative_constraint(F<Ts...> const &)
    {
        static_assert(is_applicative<F>(), "F: not a applicative!");
    }


    Test(applicative_constraint)
    {
        applicative_constraint( std::vector<std::string>{ "one", "two", "three" });
        applicative_constraint( std::list<std::string>  { "one", "two", "three" });
        applicative_constraint( std::deque<std::string>  { "one", "two", "three" });
        applicative_constraint( std::forward_list<std::string>  { "one", "two", "three" });
        applicative_constraint( std::make_shared<std::string>( "one" ));
        applicative_constraint( std::make_unique<std::string>( "one" ));
        applicative_constraint( my::applicative<std::string>  { "one" });
    }


    Test(applicative_simple)
    {
        auto f = pure.in<std::vector>(std::function<int(int)>([](int n) { return n+1; }));
        auto x = pure.in<std::vector>(10);

        auto y = f * x;
        Assert(x, is_equal_to(std::vector<int>{10}));
        Assert(y, is_equal_to(std::vector<int>{11}));
    }

    Test(applicative_apply)
    {
        auto fs = std::vector<std::function<int(int)>>
                        { [](int n) { return n+1; },
                          [](int n) { return n*2; }
                        };

        auto xs = std::vector<int>{1,2};
        auto ys = fs * xs;

        auto zs = [](int n) { return n+1; } <$> xs;

        Assert(ys, is_equal_to(std::vector<int>{2,3,2,4}));
        Assert(zs, is_equal_to(std::vector<int>{2,3}));
    }


    Test(applicative_vector)
    {
        auto x = pure.in<std::vector>(42);

        auto fs = std::vector<std::function<int(int)>> { [](int n) { return n+1; }, [](int n) { return n*2; }};
        auto xs = std::vector<int>{1,2};

        auto ys = fs * xs;

        Assert(x,  is_equal_to(std::vector<int>{42}));
        Assert(ys, is_equal_to(std::vector<int>{2,3,2,4}));
    }


    Test(applicative_deque)
    {
        auto x = pure.in<std::deque>(42);

        auto fs = std::deque<std::function<int(int)>> { [](int n) { return n+1; }, [](int n) { return n*2; }};
        auto xs = std::deque<int>{1,2};

        auto ys = fs * xs;

        Assert(x,  is_equal_to(std::deque<int>{42}));
        Assert(ys, is_equal_to(std::deque<int>{2,3,2,4}));
    }


    Test(applicative_list)
    {
        auto x = pure.in<std::list>(42);

        auto fs = std::list<std::function<int(int)>> { [](int n) { return n+1; }, [](int n) { return n*2; }};
        auto xs = std::list<int>{1,2};

        auto ys = fs * xs;

        Assert(x,  is_equal_to(std::list<int>{42}));
        Assert(ys, is_equal_to(std::list<int>{2,3,2,4}));
    }


    Test(applicative_forward_list)
    {
        auto x  = pure.in<std::forward_list>(42);
        auto fs = std::forward_list<std::function<int(int)>> { [](int n) { return n+1; }, [](int n) { return n*2; }};
        auto xs = std::forward_list<int>{1,2};

        auto ys = fs * xs;

        Assert(x,  is_equal_to(std::forward_list<int>{42}));
        Assert(ys, is_equal_to(std::forward_list<int>{2,3,2,4}));
    }


    Test(applicative_shared_ptr)
    {
        auto z = pure.in<std::shared_ptr>(42);

        auto f  = std::make_shared<std::function<int(int)>> ([](int n) { return n+1; });
        auto f_ = std::shared_ptr<std::function<int(int)>>();

        auto x  = std::make_shared<int>(41);
        auto x_ = std::shared_ptr<int>();


        auto y1 = f  * x;
        auto y2 = f_ * x;
        auto y3 = f  * x_;
        auto y4 = f_ * x_;

        Assert(z.get() != nullptr);
        Assert(y1.get() != nullptr);

        Assert(*z,   is_equal_to(42));
        Assert(*y1,  is_equal_to(42));

        Assert(!y2);
        Assert(!y3);
        Assert(!y4);
    }


    Test(applicative_unique_ptr)
    {
        auto z  = pure.in<std::unique_ptr>(42);
        auto f  = std::make_unique<std::function<int(int)>> ([](int n) { return n+1; });
        auto f_ = std::unique_ptr<std::function<int(int)>>();
        auto x  = std::make_unique<int>(41);
        auto x_ = std::unique_ptr<int>{};

        auto y1 = f  * x;
        auto y2 = f_ * x;
        auto y3 = f  * x_;
        auto y4 = f_ * x_;

        Assert(z.get() != nullptr);
        Assert(y1.get() != nullptr);

        Assert(*z,   is_equal_to(42));
        Assert(*y1,  is_equal_to(42));

        Assert(!y2);
        Assert(!y3);
        Assert(!y4);
    }


    Test(applicative_optional)
    {
        auto z  = pure.in<optional>(42);

        auto f  = make_optional<std::function<int(int)>> ([](int n) { return n+1; });
        auto f_ = optional<std::function<int(int)>>();

        auto x  = make_optional<int>(41);
        auto x_ = optional<int>();

        auto y1 = f  * x;
        auto y2 = f_ * x;
        auto y3 = f  * x_;
        auto y4 = f_ * x_;

        Assert(static_cast<bool>(z));
        Assert(static_cast<bool>(y1));

        Assert(z.value(),  is_equal_to(42));
        Assert(y1.value(), is_equal_to(42));

        Assert(!y2);
        Assert(!y3);
        Assert(!y4);
    }

    Test(applicative_future)
    {
        auto z  = pure.in<std::future>(42);

        Assert(z.get() == 42 );

        auto x  = pure.in<std::future>(42);
        auto f  = pure.in<std::future>(currying([](int n) { return n+1; }));

        auto r  = std::move(f) * std::move(x);  // std::future is non-copyable!

        Assert( r.get(), is_equal_to(43) );
    }


    int sum_f(int a, int b) { return a + b; };


    Test(applicative_currying)
    {
        auto sum = currying(sum_f);

        auto x = make_optional(21);

        auto a  = std::vector<int>{ 1, 2 };
        auto b  = std::vector<int>{ 0, 1, 2};

        Assert( (sum <$> x) * x == 42);

        auto n = (sum <$> a) * b;

        Assert( n == std::vector<int>{ 1, 2, 3, 2, 3, 4, } );
    };


    Test(alternative_simple)
    {
        Assert( empty<optional<int>>() == nullopt);
        Assert( or_(empty<optional<int>>(), make_optional(42)).value() == 42 );
        Assert( (empty<optional<int>>() || make_optional(42)).value() == 42);
        Assert( (make_optional(42) || empty<optional<int>>()).value() == 42);
    }

    Test(alternative_vector)
    {
        Assert( empty<std::vector<int>>() == std::vector<int>{});
        Assert( or_(empty<std::vector<int>>(), std::vector<int>{42}) == pure.in<std::vector>(42) );
        Assert( or_(std::vector<int>{42}, empty<std::vector<int>>()) == pure.in<std::vector>(42) );
        Assert( or_(std::vector<int>{42}, std::vector<int>{11}) == std::vector<int>{42, 11} );

        Assert( (empty<std::vector<int>>() || std::vector<int>{42} ) == pure.in<std::vector>(42) );
        Assert( (std::vector<int>{42} || empty<std::vector<int>>() ) == pure.in<std::vector>(42) );
        Assert( (std::vector<int>{42} || std::vector<int>{11})  == std::vector<int>{42, 11} );
    }

    Test(alternative_list)
    {
        Assert( empty<std::list<int>>() == std::list<int>{});
        Assert( or_(empty<std::list<int>>(), std::list<int>{42}) == pure.in<std::list>(42) );
        Assert( or_(std::list<int>{42}, empty<std::list<int>>()) == pure.in<std::list>(42) );
        Assert( or_(std::list<int>{42}, std::list<int>{11}) == std::list<int>{42, 11} );

        Assert( (empty<std::list<int>>() || std::list<int>{42} ) == pure.in<std::list>(42) );
        Assert( (std::list<int>{42} || empty<std::list<int>>() ) == pure.in<std::list>(42) );
        Assert( (std::list<int>{42} || std::list<int>{11})  == std::list<int>{42, 11} );
    }

    Test(alternative_deque)
    {
        Assert( empty<std::deque<int>>() == std::deque<int>{});
        Assert( or_(empty<std::deque<int>>(), std::deque<int>{42}) == pure.in<std::deque>(42) );
        Assert( or_(std::deque<int>{42}, empty<std::deque<int>>()) == pure.in<std::deque>(42) );
        Assert( or_(std::deque<int>{42}, std::deque<int>{11}) == std::deque<int>{42, 11} );

        Assert( (empty<std::deque<int>>() || std::deque<int>{42} ) == pure.in<std::deque>(42) );
        Assert( (std::deque<int>{42} || empty<std::deque<int>>() ) == pure.in<std::deque>(42) );
        Assert( (std::deque<int>{42} || std::deque<int>{11})  == std::deque<int>{42, 11} );
    }


    Test(alternative_forward_list)
    {
        Assert( empty<std::forward_list<int>>() == std::forward_list<int>{});
        Assert( or_(empty<std::forward_list<int>>(), std::forward_list<int>{42}) == pure.in<std::forward_list>(42) );
        Assert( or_(std::forward_list<int>{42}, empty<std::forward_list<int>>()) == pure.in<std::forward_list>(42) );
        Assert( or_(std::forward_list<int>{42}, std::forward_list<int>{11}) == std::forward_list<int>{42, 11} );

        Assert( (empty<std::forward_list<int>>() || std::forward_list<int>{42} ) == pure.in<std::forward_list>(42) );
        Assert( (std::forward_list<int>{42} || empty<std::forward_list<int>>() ) == pure.in<std::forward_list>(42) );
        Assert( (std::forward_list<int>{42} || std::forward_list<int>{11})  == std::forward_list<int>{42, 11} );
    }

    Test(alternative_shared_ptr)
    {
        Assert( empty<std::shared_ptr<int>>() == std::shared_ptr<int>{});

        Assert( * or_(empty<std::shared_ptr<int>>(), std::make_shared<int>(42)) == 42 );
        Assert( * or_(std::make_shared<int>(42), empty<std::shared_ptr<int>>()) == 42 );
        Assert( * or_(std::make_shared<int>(42), std::make_shared<int>(11))     == 42 );

        Assert( * (empty<std::shared_ptr<int>>() || std::make_shared<int>(42) ) == 42 );
        Assert( * (std::make_shared<int>(42) || empty<std::shared_ptr<int>>() ) == 42 );
        Assert( * (std::make_shared<int>(42) || std::make_shared<int>(11))      == 42 );
    }

    Test(alternative_unique_ptr)
    {
        Assert( empty<std::unique_ptr<int>>() == std::unique_ptr<int>{});

        Assert( * or_(empty<std::unique_ptr<int>>(), std::make_unique<int>(42)) == 42 );
        Assert( * or_(std::make_unique<int>(42), empty<std::unique_ptr<int>>()) == 42 );
        Assert( * or_(std::make_unique<int>(42), std::make_unique<int>(11))     == 42 );

        Assert( * (empty<std::unique_ptr<int>>() || std::make_unique<int>(42) ) == 42 );
        Assert( * (std::make_unique<int>(42) || empty<std::unique_ptr<int>>() ) == 42 );
        Assert( * (std::make_unique<int>(42) || std::make_unique<int>(11))      == 42 );
    }

}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

