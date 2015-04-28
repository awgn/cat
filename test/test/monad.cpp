#include <cat/monad.hpp>
#include <cat/bits/type.hpp>
#include <yats.hpp>

using namespace yats;
using namespace cat;
using namespace std::literals;


// Tests:
//

Context(monad)
{
    template <template <typename...> class F, typename ...Ts>
    void monad_constraint(F<Ts...> const &)
    {
        static_assert(is_monad<F>::value, "F: not a monad!");
    }


    Test(monad_optional)
    {
        auto f = [](int n) { return mreturn.in<optional>(n); };
        auto g = [](int n) { return mreturn.in<optional>(n+2); };
        auto h = [](int  ) { return optional<int>{}; };

        Assert( mbind(mreturn.in<optional>(10), f) == 10 );

        Assert( mbind( mbind(mreturn.in<optional>(10), f), g) == 12 );

        Assert( (mreturn.in<optional>(10) >>= f)  == 10 );

        Assert( ((mreturn.in<optional>(10)
                 >>= f) >>= g) == 12 );

        Assert( ((mreturn.in<optional>(10)
                 >>= h) >>= g).value_or(42) == 42 );
    }


    Test(monad_vector)
    {
        std::vector<int> v { 1, 2, 3 };
        auto f = [](int n) { return std::vector<int> {n, n}; };

        Assert( (v >>= f) == std::vector<int>{1, 1, 2, 2, 3, 3} );
        Assert( ((v >>= f) >>= f) == std::vector<int>{1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3} );

        std::vector<int> q { 1, 2 };

        Assert( (q >> v) == std::vector<int> {1, 2, 3, 1, 2, 3});
    }

    Test(monad_string)
    {
        std::string v {"123"};

        auto f = [](char c) { return std::string(2,c); };

        Assert( (v >>= f) == std::string("112233") );
        Assert( ((v >>= f) >>= f) == std::string("111122223333") );

        std::string q{"12"};

        Assert( (q >> v) == std::string("123123"));
    }

    Test(monad_deque)
    {
        std::deque<int> v { 1, 2, 3 };
        auto f = [](int n) { return std::deque<int> {n, n}; };

        Assert( (v >>= f) == std::deque<int>{1, 1, 2, 2, 3, 3} );
        Assert( ((v >>= f) >>= f) == std::deque<int>{1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3} );
    }


    Test(monad_list)
    {
        std::list<int> v { 1, 2, 3 };
        auto f = [](int n) { return std::list<int> {n, n}; };

        Assert( (v >>= f) == std::list<int>{1, 1, 2, 2, 3, 3} );
        Assert( ((v >>= f) >>= f) == std::list<int>{1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3} );
    }


    Test(monad_shared_ptr)
    {
        auto f = [](int n) { return mreturn.in<std::shared_ptr>(n); };
        auto g = [](int n) { return mreturn.in<std::shared_ptr>(n+2); };
        auto h = [](int  ) { return std::shared_ptr<int>{}; };

        Assert( *mbind(mreturn.in<std::shared_ptr>(10), f) == 10 );
        Assert( *mbind( mbind(mreturn.in<std::shared_ptr>(10), f), g) == 12 );

        Assert( *(mreturn.in<std::shared_ptr>(10) >>= f)  == 10 );

        Assert( *((mreturn.in<std::shared_ptr>(10)
                 >>= f) >>= g) == 12 );

        Assert( not ((mreturn.in<std::shared_ptr>(10)
                 >>= h) >>= g) );
    }


    Test(monad_future)
    {
        auto f = [](int n) { return mreturn.in<std::future>(n); };
        auto g = [](int n) { return mreturn.in<std::future>(n+2); };

        Assert( mbind(mreturn.in<std::future>(10), f).get() == 10 );

        Assert( mbind( mbind(mreturn.in<std::future>(10), f), g).get() == 12 );

        Assert( (mreturn.in<std::future>(10) >>= f).get()  == 10 );

        Assert( ((mreturn.in<std::future>(10)
                 >>= f) >>= g).get() == 12 );
    }


    Test(monad_unique_ptr)
    {
        auto f = [](int n) { return mreturn.in<std::unique_ptr>(n); };
        auto g = [](int n) { return mreturn.in<std::unique_ptr>(n+2); };
        auto h = [](int  ) { return std::unique_ptr<int>{}; };


        Assert( *mbind(mreturn.in<std::unique_ptr>(10), f) == 10 );
        Assert( *mbind( mbind(mreturn.in<std::unique_ptr>(10), f), g) == 12 );

        Assert( *(mreturn.in<std::unique_ptr>(10) >>= f)  == 10 );

        Assert( *((mreturn.in<std::unique_ptr>(10)
                 >>= f) >>= g) == 12 );

        Assert( not ((mreturn.in<std::unique_ptr>(10)
                 >>= h) >>= g) );
    }


    Test(monad_constraint)
    {
        monad_constraint( make_optional<std::string>( "one" ));
        monad_constraint( std::vector<std::string>{} );
        monad_constraint( std::deque<std::string>{} );
        monad_constraint( std::list<std::string>  { "one", "two", "three" }); monad_constraint( std::make_shared<std::string>( "one" ));
        monad_constraint( std::make_unique<std::string>( "one" ));
    }


    Test(monad_sequence)
    {
        std::list< std::vector<int> > l = { {1}, {1,2}, {1,2,3} };
        std::vector< std::list<int> > expected = { {1,1,1}, {1,1,2}, {1,1,3}, {1,2,1}, {1,2,2}, {1,2,3} };

        Assert(sequence(l) == expected);

        std::list< optional<int> > l2 = { make_optional(1), make_optional(2) };
        Assert( sequence(l2) == make_optional(std::list<int>{1,2}) );

        std::list< optional<int> > l3 = { nullopt, make_optional(2) };
        Assert( sequence(l3) == nullopt );

        std::list< optional<int> > l4 = { make_optional(1), nullopt };
        Assert( sequence(l4) == nullopt );
    }


    Test(kleisli)
    {
        auto f = [](int n) { return mreturn.in<std::unique_ptr>(n); };
        auto g = [](int n) { return mreturn.in<std::unique_ptr>(n+2); };

        Assert( *((mreturn.in<std::unique_ptr>(10) >>= f) >>= g) == 12 );

        auto h = currying(f) <kleisli> g;

        Assert( *(mreturn.in<std::unique_ptr>(10) >>= h) == 12 );
    }


    Test(for_and_map)
    {
        std::list<int> xs = { 1, 2, 3 };
        auto f = [] (int n) { return std::vector<std::string> { std::to_string(n) }; };

        Assert( mapM(f, xs) == std::vector< std::list<std::string> >{{"1", "2", "3" }} );

        auto r = forM(xs, []( int n) {
             return mreturn.in<std::vector>( std::to_string(n) );
        });

        Assert( r == std::vector< std::list<std::string> >{{"1", "2", "3" }} );

        std::list<int> ys = {1, 2};

        auto z = forM(ys, [](int n)
        {
            return std::vector<std::string>{ std::to_string(n), std::to_string(n) + "!" };
        });

        Assert( z == std::vector<std::list<std::string> > {
                { "1", "2" },
                { "1", "2!" },
                { "1!", "2" },
                { "1!", "2!" }
       });
    }

    /////////////////////// Monad Plus

    Test(monadplus_optional)
    {
        auto n = mzero<optional<int>>();

        auto x = mreturn.in<optional>(1);
        auto y = mreturn.in<optional>(2);

        Assert ( n == nullopt);

        Assert ( mplus (n, x) == 1 );
        Assert ( mplus (x, n) == 1 );
        Assert ( mplus (x, y) == 1 );
        Assert ( mplus (y, x) == 2 );
    }

    Test(monadplus_shared_ptr)
    {
        auto n = mzero<std::shared_ptr<int>>();

        auto x = mreturn.in<std::shared_ptr>(1);
        auto y = mreturn.in<std::shared_ptr>(2);

        Assert ( not n );

        Assert ( * mplus (n, x) == 1 );
        Assert ( * mplus (x, n) == 1 );
        Assert ( * mplus (x, y) == 1 );
        Assert ( * mplus (y, x) == 2 );
    }

    Test(monadplus_unique_ptr)
    {
        auto n = mzero<std::unique_ptr<int>>();

        auto x = mreturn.in<std::unique_ptr>(1);
        auto y = mreturn.in<std::unique_ptr>(2);

        Assert ( not n );

        Assert ( * mplus (n, x) == 1 );
        Assert ( * mplus (x, n) == 1 );
        Assert ( * mplus (x, y) == 1 );

        Assert ( * mplus (std::move(y), std::move(x)) == 2 );
    }


    Test(monadplus_vector)
    {
        auto n = mzero<std::vector<int>>();
        auto x = std::vector<int> {1, 2, 3};
        auto y = std::vector<int> {4, 5, 6};

        Assert( mplus(n, n) == mzero<std::vector<int>>());
        Assert( mplus(n, x) == std::vector<int>{1, 2, 3});
        Assert( mplus(x, n) == std::vector<int>{1, 2, 3});
        Assert( mplus(x, y) == std::vector<int>{1, 2, 3, 4, 5, 6});
        Assert( mplus(y, x) == std::vector<int>{4, 5, 6, 1, 2, 3});
    }

    Test(monadplus_deque)
    {
        auto n = mzero<std::deque<int>>();
        auto x = std::deque<int> {1, 2, 3};
        auto y = std::deque<int> {4, 5, 6};

        Assert( mplus(n, n) == mzero<std::deque<int>>());
        Assert( mplus(n, x) == std::deque<int>{1, 2, 3});
        Assert( mplus(x, n) == std::deque<int>{1, 2, 3});
        Assert( mplus(x, y) == std::deque<int>{1, 2, 3, 4, 5, 6});
        Assert( mplus(y, x) == std::deque<int>{4, 5, 6, 1, 2, 3});
    }

    Test(monadplus_list)
    {
        auto n = mzero<std::list<int>>();
        auto x = std::list<int> {1, 2, 3};
        auto y = std::list<int> {4, 5, 6};

        Assert( mplus(n, n) == mzero<std::list<int>>());
        Assert( mplus(n, x) == std::list<int>{1, 2, 3});
        Assert( mplus(x, n) == std::list<int>{1, 2, 3});
        Assert( mplus(x, y) == std::list<int>{1, 2, 3, 4, 5, 6});
        Assert( mplus(y, x) == std::list<int>{4, 5, 6, 1, 2, 3});
    }


    Test (other)
    {
        std::unique_ptr<std::unique_ptr<int>> p = std::make_unique< std::unique_ptr<int> >(std::make_unique<int>(42));
        Assert(*join(std::move(p)), is_equal_to(42));

        std::list< std::vector<int> > list = { {}, {1}, {2,3} };
        Assert( msum(list) == std::vector<int> {1,2,3});

        Assert( (guard<optional<int>>(false) >> make_optional(1) ) == nullopt );
        Assert( (guard<optional<int>>(true) >> make_optional("ok"s) ) == "ok"s );
    }
}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

