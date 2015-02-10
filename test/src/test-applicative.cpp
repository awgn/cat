#include <cat/applicative.hpp>

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
    template <template <typename...> class F, typename T>
    void applicative_constraint(F<T> const &)
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


    Test(simple)
    {
        auto f = pure<std::vector>(std::function<int(int)>([](int n) { return n+1; }));
        auto x = pure<std::vector>(10);

        auto y = f * x;
        Assert(x, is_equal_to(std::vector<int>{10}));
    }


    Test(apply)
    {
        auto fs = std::vector<std::function<int(int)>>
                        { [](int n) { return n+1; },
                          [](int n) { return n*2; }
                        };

        auto xs = std::vector<int>{1,2};

        auto ys = fs * xs;

        Assert(ys, is_equal_to(std::vector<int>{2,3,2,4}));

    }


    Test(vector)
    {
        auto x = pure<std::vector>(42);

        auto fs = std::vector<std::function<int(int)>> { [](int n) { return n+1; },
                                                         [](int n) { return n*2; }};
        auto xs = std::vector<int>{1,2};

        auto ys = fs * xs;

        Assert(x,  is_equal_to(std::vector<int>{42}));
        Assert(ys, is_equal_to(std::vector<int>{2,3,2,4}));
    }


    Test(deque)
    {
        auto x = pure<std::deque>(42);

        auto fs = std::deque<std::function<int(int)>> { [](int n) { return n+1; },
                                                         [](int n) { return n*2; }};
        auto xs = std::deque<int>{1,2};

        auto ys = fs * xs;

        Assert(x,  is_equal_to(std::deque<int>{42}));
        Assert(ys, is_equal_to(std::deque<int>{2,3,2,4}));
    }


    Test(list)
    {
        auto x = pure<std::list>(42);

        auto fs = std::list<std::function<int(int)>> { [](int n) { return n+1; },
                                                         [](int n) { return n*2; }};
        auto xs = std::list<int>{1,2};

        auto ys = fs * xs;

        Assert(x,  is_equal_to(std::list<int>{42}));
        Assert(ys, is_equal_to(std::list<int>{2,3,2,4}));
    }


    Test(forward_list)
    {
        auto x = pure<std::forward_list>(42);

        auto fs = std::forward_list<std::function<int(int)>> { [](int n) { return n+1; }, [](int n) { return n*2; }};
        auto xs = std::forward_list<int>{1,2};

        auto ys = fs * xs;

        Assert(x,  is_equal_to(std::forward_list<int>{42}));
        Assert(ys, is_equal_to(std::forward_list<int>{2,3,2,4}));
    }


    Test(shared_ptr)
    {
        auto z = pure<std::shared_ptr>(42);

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


    Test(unique_ptr)
    {
        auto z  = pure<std::unique_ptr>(42);
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


    Test(optional)
    {
        auto z  = pure<std::experimental::optional>(42);

        auto f  = std::experimental::make_optional<std::function<int(int)>> ([](int n) { return n+1; });
        auto f_ = std::experimental::optional<std::function<int(int)>>();

        auto x  = std::experimental::make_optional<int>(41);
        auto x_ = std::experimental::optional<int>();


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

}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

