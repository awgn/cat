#include <cat/monad.hpp>
#include <cat/utility/typeof.hpp>
#include <yats.hpp>

using namespace yats;
using namespace cat;


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
        auto f = [](int n) { return mreturn<std::experimental::optional>(n); };
        auto g = [](int n) { return mreturn<std::experimental::optional>(n+2); };
        auto h = [](int  ) { return std::experimental::optional<int>{}; };

        Assert( mbind(mreturn<std::experimental::optional>(10), f) == 10 );
        Assert( mbind( mbind(mreturn<std::experimental::optional>(10), f), g) == 12 );

        Assert( (mreturn<std::experimental::optional>(10) >>= f)  == 10 );

        Assert( ((mreturn<std::experimental::optional>(10)
                 >>= f) >>= g) == 12 );

        Assert( ((mreturn<std::experimental::optional>(10)
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
        auto f = [](int n) { return mreturn<std::shared_ptr>(n); };
        auto g = [](int n) { return mreturn<std::shared_ptr>(n+2); };
        auto h = [](int  ) { return std::shared_ptr<int>{}; };


        Assert( *mbind(mreturn<std::shared_ptr>(10), f) == 10 );
        Assert( *mbind( mbind(mreturn<std::shared_ptr>(10), f), g) == 12 );

        Assert( *(mreturn<std::shared_ptr>(10) >>= f)  == 10 );

        Assert( *((mreturn<std::shared_ptr>(10)
                 >>= f) >>= g) == 12 );

        Assert( not ((mreturn<std::shared_ptr>(10)
                 >>= h) >>= g) );
    }


    Test(monad_unique_ptr)
    {
        auto f = [](int n) { return mreturn<std::unique_ptr>(n); };
        auto g = [](int n) { return mreturn<std::unique_ptr>(n+2); };
        auto h = [](int  ) { return std::unique_ptr<int>{}; };


        Assert( *mbind(mreturn<std::unique_ptr>(10), f) == 10 );
        Assert( *mbind( mbind(mreturn<std::unique_ptr>(10), f), g) == 12 );

        Assert( *(mreturn<std::unique_ptr>(10) >>= f)  == 10 );

        Assert( *((mreturn<std::unique_ptr>(10)
                 >>= f) >>= g) == 12 );

        Assert( not ((mreturn<std::unique_ptr>(10)
                 >>= h) >>= g) );
    }


    Test(monad_constraint)
    {
        monad_constraint( std::experimental::make_optional<std::string>( "one" ));
        monad_constraint( std::vector<std::string>{} );
        monad_constraint( std::deque<std::string>{} );
        monad_constraint( std::list<std::string>  { "one", "two", "three" });
        monad_constraint( std::make_shared<std::string>( "one" ));
        monad_constraint( std::make_unique<std::string>( "one" ));
    }

    Test(monad_sequence)
    {
        std::list< std::vector<int> > l = { {1}, {1,2}, {1,2,3} };
        std::vector< std::list<int> > expected = { {1,1,1}, {1,1,2}, {1,1,3}, {1,2,1}, {1,2,2}, {1,2,3} };

        Assert(sequence(l) == expected);

        std::list< std::experimental::optional<int> > l2 = { std::experimental::make_optional(1), std::experimental::make_optional(2) };
        Assert( sequence(l2) == std::experimental::make_optional(std::list<int>{1,2}) );

        std::list< std::experimental::optional<int> > l3 = { std::experimental::nullopt, std::experimental::make_optional(2) };
        Assert( sequence(l3) == std::experimental::nullopt );

        std::list< std::experimental::optional<int> > l4 = { std::experimental::make_optional(1), std::experimental::nullopt };
        Assert( sequence(l4) == std::experimental::nullopt );
    }


    Test(kleisli)
    {
        auto f = [](int n) { return mreturn<std::unique_ptr>(n); };
        auto g = [](int n) { return mreturn<std::unique_ptr>(n+2); };

        Assert( *((mreturn<std::unique_ptr>(10) >>= f) >>= g) == 12 );

        auto h = callable(f) <k> g;

        Assert( *(mreturn<std::unique_ptr>(10) >>= h) == 12 );
    }
}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

