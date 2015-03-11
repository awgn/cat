#include <cat/foldable.hpp>
#include <cat/monoid.hpp>

#include <yats.hpp>

using namespace yats;
using namespace cat;


Context(foldable)
{

    template <template <typename...> class F, typename ...Ts>
    void foldable_constraint(F<Ts...> const &)
    {
        static_assert(is_foldable<F>::value, "F: not a foldable!");
    }


    Test(foldable_vector)
    {
        Assert( foldr( [](int a, int b)  { return a - b;}, 0, std::vector<int>{1,2,3}) , is_equal_to( 2) );
        Assert( foldl( [](int a, int b)  { return a - b;}, 0, std::vector<int>{1,2,3}) , is_equal_to(-6) );
        Assert( foldl1( [](int a, int b) { return a - b;}, std::vector<int>{1,2,3}) , is_equal_to(-4) );
        Assert( foldr1( [](int a, int b) { return a - b;}, std::vector<int>{1,2,3}) , is_equal_to( 2) );

        Assert( fold(std::vector<Any>{ Any{true}, Any{false}, Any{false} }) == (Any{true}) );
        Assert( fold(std::vector<All>{ All{true}, All{false}, All{false} }) == (All{false}) );
    }

    Test(foldable_list)
    {
        Assert( foldr( [](int a, int b)  { return a - b;}, 0, std::list<int>{1,2,3}) , is_equal_to( 2) );
        Assert( foldl( [](int a, int b)  { return a - b;}, 0, std::list<int>{1,2,3}) , is_equal_to(-6) );
        Assert( foldl1( [](int a, int b) { return a - b;}, std::list<int>{1,2,3}) , is_equal_to(-4) );
        Assert( foldr1( [](int a, int b) { return a - b;}, std::list<int>{1,2,3}) , is_equal_to( 2) );

        Assert( fold(std::list<Any>{ Any{true}, Any{false}, Any{false} }) == (Any{true}) );
        Assert( fold(std::list<All>{ All{true}, All{false}, All{false} }) == (All{false}) );
    }

    Test(foldable_deque)
    {
        Assert( foldr( [](int a, int b)  { return a - b;}, 0, std::deque<int>{1,2,3}) , is_equal_to( 2) );
        Assert( foldl( [](int a, int b)  { return a - b;}, 0, std::deque<int>{1,2,3}) , is_equal_to(-6) );
        Assert( foldl1( [](int a, int b) { return a - b;}, std::deque<int>{1,2,3}) , is_equal_to(-4) );
        Assert( foldr1( [](int a, int b) { return a - b;}, std::deque<int>{1,2,3}) , is_equal_to( 2) );

        Assert( fold(std::deque<Any>{ Any{true}, Any{false}, Any{false} }) == (Any{true}) );
        Assert( fold(std::deque<All>{ All{true}, All{false}, All{false} }) == (All{false}) );
    }


    Test(foldable_forward_list)
    {
        Assert( foldr( [](int a, int b)  { return a - b;}, 0, std::forward_list<int>{1,2,3}) , is_equal_to( 2) );
        Assert( foldl( [](int a, int b)  { return a - b;}, 0, std::forward_list<int>{1,2,3}) , is_equal_to(-6) );
        Assert( foldl1( [](int a, int b) { return a - b;}, std::forward_list<int>{1,2,3}) , is_equal_to(-4) );
        Assert( foldr1( [](int a, int b) { return a - b;}, std::forward_list<int>{1,2,3}) , is_equal_to( 2) );

        Assert( fold(std::forward_list<Any>{ Any{true}, Any{false}, Any{false} }) == (Any{true}) );
        Assert( fold(std::forward_list<All>{ All{true}, All{false}, All{false} }) == (All{false}) );
    }


    Test(foldable_optional)
    {
        Assert( foldr( [](int a, int b)  { return a - b;}, 0, make_optional<int>(1)) , is_equal_to(1) );
        Assert( foldl( [](int a, int b)  { return a - b;}, 0, make_optional<int>(1)) , is_equal_to(-1) );

        Assert( foldr( [](int a, int b)  { return a - b;}, 42, optional<int>()), is_equal_to(42) );
        Assert( foldl( [](int a, int b)  { return a - b;}, 42, optional<int>()), is_equal_to(42) );

        AssertThrow( foldr1( [](int a, int b)  { return a - b;}, optional<int>()) );
        AssertThrow( foldl1( [](int a, int b)  { return a - b;}, optional<int>()) );
    }


    Test(foldable_pair)
    {
        Assert( foldr( [](int a, int b)  { return a + b;}, 1, std::make_pair("hello", 1)) , is_equal_to(2) );
        Assert( foldl( [](int a, int b)  { return a + b;}, 1, std::make_pair("world", 1)) , is_equal_to(2) );

        Assert( foldr1( [](int a, int b)  { return a + b;}, std::make_pair("hello", 42)) , is_equal_to(42) );
        Assert( foldl1( [](int a, int b)  { return a + b;}, std::make_pair("world", 42)) , is_equal_to(42) );
    }


    Test(foldable_unique_ptr)
    {
        Assert( foldr( [](int a, int b)  { return a - b;}, 0, std::make_unique<int>(1)) , is_equal_to(1) );
        Assert( foldl( [](int a, int b)  { return a - b;}, 0, std::make_unique<int>(1)) , is_equal_to(-1) );

        Assert( foldr( [](int a, int b)  { return a - b;}, 42, std::unique_ptr<int>()), is_equal_to(42) );
        Assert( foldl( [](int a, int b)  { return a - b;}, 42, std::unique_ptr<int>()), is_equal_to(42) );

        AssertThrow( foldr1( [](int a, int b)  { return a - b;}, std::unique_ptr<int>()) );
        AssertThrow( foldl1( [](int a, int b)  { return a - b;}, std::unique_ptr<int>()) );
    }


    Test(foldable_shared_ptr)
    {
        Assert( foldr( [](int a, int b)  { return a - b;}, 0, std::make_shared<int>(1)) , is_equal_to(1) );
        Assert( foldl( [](int a, int b)  { return a - b;}, 0, std::make_shared<int>(1)) , is_equal_to(-1) );

        Assert( foldr( [](int a, int b)  { return a - b;}, 42, std::shared_ptr<int>()), is_equal_to(42) );
        Assert( foldl( [](int a, int b)  { return a - b;}, 42, std::shared_ptr<int>()), is_equal_to(42) );

        AssertThrow( foldr1( [](int a, int b)  { return a - b;}, std::shared_ptr<int>()) );
        AssertThrow( foldl1( [](int a, int b)  { return a - b;}, std::shared_ptr<int>()) );
    }


    Test(foldable_map)
    {
        Assert( foldr([](int a, int b)   { return a + b;}, 0, std::map<std::string, int>{{"one", 1}, {"two", 2}, {"three", 3}}) , is_equal_to(6) );
        Assert( foldl( [](int a, int b)  { return a + b;}, 0, std::map<std::string, int>{{"one", 1},{"two", 2}, { "three", 3}}) , is_equal_to(6) );
        Assert( foldr1( [](int a, int b) { return a + b;}, std::map<std::string, int>{{"one", 1}, {"two", 2}, {"three", 3}}) , is_equal_to(6) );
        Assert( foldl1( [](int a, int b) { return a + b;}, std::map<std::string, int>{{"one", 1}, {"two", 2}, {"three", 3}}) , is_equal_to(6) );
    }


    Test(foldable_multimap)
    {
        Assert( foldr([](int a, int b)   { return a + b;}, 0, std::multimap<std::string, int>{{"one", 1}, {"two", 2}, {"three", 3}}) , is_equal_to(6) );
        Assert( foldl( [](int a, int b)  { return a + b;}, 0, std::multimap<std::string, int>{{"one", 1},{"two", 2}, { "three", 3}}) , is_equal_to(6) );
        Assert( foldr1( [](int a, int b) { return a + b;}, std::multimap<std::string, int>{{"one", 1}, {"two", 2}, {"three", 3}}) , is_equal_to(6) );
        Assert( foldl1( [](int a, int b) { return a + b;}, std::multimap<std::string, int>{{"one", 1}, {"two", 2}, {"three", 3}}) , is_equal_to(6) );
    }


    Test(foldable_set)
    {
        Assert( foldr( [](int a, int b)  { return a - b;}, 0, std::set<int>{1,2,3}) , is_equal_to( 2) );
        Assert( foldl( [](int a, int b)  { return a - b;}, 0, std::set<int>{1,2,3}) , is_equal_to(-6) );
        Assert( foldl1( [](int a, int b) { return a - b;}, std::set<int>{1,2,3}) , is_equal_to(-4) );
        Assert( foldr1( [](int a, int b) { return a - b;}, std::set<int>{1,2,3}) , is_equal_to( 2) );
    }


    Test(foldable_multiset)
    {
        Assert( foldr( [](int a, int b)  { return a - b;}, 0, std::multiset<int>{1,2,3}) , is_equal_to( 2) );
        Assert( foldl( [](int a, int b)  { return a - b;}, 0, std::multiset<int>{1,2,3}) , is_equal_to(-6) );
        Assert( foldl1( [](int a, int b) { return a - b;}, std::multiset<int>{1,2,3}) , is_equal_to(-4) );
        Assert( foldr1( [](int a, int b) { return a - b;}, std::multiset<int>{1,2,3}) , is_equal_to( 2) );
    }



    Test(foldable_constraint)
    {
        foldable_constraint( std::vector<std::string>{} );
        foldable_constraint( std::deque<std::string>{} );
        foldable_constraint( std::list<std::string>  { "one", "two", "three" });
        foldable_constraint( std::forward_list<std::string>  { "one", "two", "three" });
        foldable_constraint( std::make_shared<std::string>( "one" ));
        foldable_constraint( std::make_unique<std::string>( "one" ));
        foldable_constraint( make_optional<std::string>( "one" ));
        foldable_constraint( std::map<std::string, int>{} );
        foldable_constraint( std::multimap<std::string, int>{} );
        foldable_constraint( std::set<int>{} );
        foldable_constraint( std::multiset<int>{} );
    }

}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

