#include <cat/monoid.hpp>
#include <yats.hpp>

#include <list>


using namespace yats;
using namespace cat;

Context(monoid)
{
    Test(vector)
    {
        Assert( mempty<std::vector<int>>() == std::vector<int>{} );

        auto app = mappend(std::vector<int> {1}, std::vector<int>{2}) ;
        Assert ((app == std::vector<int>{1, 2}));

        auto l = std::list<std::vector<int>>{ std::vector<int>{}, std::vector<int>{1}, std::vector<int>{2, 3} };

        auto x = mconcat(l);

        Assert((x == std::vector<int>{1,2,3}));

    }


    Test(list)
    {
        Assert( mempty<std::list<int>>() == std::list<int>{} );

        auto app = mappend(std::list<int> {1}, std::list<int>{2}) ;
        Assert ((app == std::list<int>{1, 2}));

        auto l = std::list<std::list<int>>{ std::list<int>{}, std::list<int>{1}, std::list<int>{2, 3} };

        Assert ((mconcat(l) == std::list<int>{1,2,3}));
    }


    Test(forward_list)
    {
        Assert( mempty<std::forward_list<int>>() == std::forward_list<int>{} );

        auto app = mappend(std::forward_list<int> {1}, std::forward_list<int>{2}) ;
        Assert ((app == std::forward_list<int>{1, 2}));

        auto l = std::forward_list<std::forward_list<int>>{ std::forward_list<int>{}, std::forward_list<int>{1, 2}, std::forward_list<int>{3, 4, 5} };

        Assert ((mconcat(l) == std::forward_list<int>{1,2,3,4,5}));
    }


    Test(deque)
    {
        Assert( mempty<std::deque<int>>() == std::deque<int>{} );

        auto app = mappend(std::deque<int> {1}, std::deque<int>{2}) ;
        Assert ((app == std::deque<int>{1, 2}));

        auto l = std::deque<std::deque<int>>{ std::deque<int>{}, std::deque<int>{1}, std::deque<int>{2, 3} };

        Assert ((mconcat(l) == std::deque<int>{1,2,3}));
    }


    Test(Any)
    {
        Assert( mempty<Any>() == Any{ false });
        Assert( !mempty<Any>() );

        Assert( static_cast<bool>(mappend(Any{false}, Any{true})) );
        Assert( static_cast<bool>(mappend(Any{false}, Any{true})) );

        Assert((  static_cast<bool>(mconcat( std::vector<Any>{ Any{true}, Any{false}, Any{false} })) ));
        Assert(( !static_cast<bool>(mconcat( std::vector<Any>{ Any{false}, Any{false}, Any{false} })) ));
    }


    Test(All)
    {
        Assert( mempty<All>() == All{ true });
        Assert( static_cast<bool>(mempty<All>()) );

        Assert( !static_cast<bool>(mappend(All{false}, All{true})) );
        Assert( !static_cast<bool>(mappend(All{false}, All{true})) );
        Assert( static_cast<bool>(mappend(All{true}, All{true})) );

        Assert(( !static_cast<bool>(mconcat( std::vector<All>{ All{true}, All{false}, All{false} })) ));
        Assert((  static_cast<bool>(mconcat( std::vector<All>{ All{true}, All{true}, All{true} })) ));
    }


    // Test(optional)
    // {
    //     auto x = std::experimental::make_optional<std::string>("one");
    //     auto y = std::experimental::optional<std::string>();

    //     auto r1 = fmap([](const std::string &s) { return s.size(); }, x);
    //     auto r2 = fmap([](const std::string &s) { return s.size(); }, y);

    //     Assert(static_cast<bool>(r1) == true);
    //     Assert(static_cast<bool>(r2) == false);

    //     Assert(r1.value(), is_equal_to(3));
    // }

}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}
