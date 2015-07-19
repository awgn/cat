#include <cat/monoid.hpp>
#include <yats.hpp>

#include <list>

using namespace yats;
using namespace cat;

auto g = Group("monoid")

    .Single("monoid_vector", []
    {
        Assert( mempty<std::vector<int>>() == std::vector<int>{} );

        auto app = mappend(std::vector<int> {1}, std::vector<int>{2}) ;
        Assert (app == std::vector<int>{1, 2});

        auto l = std::list<std::vector<int>>{ std::vector<int>{}, std::vector<int>{1}, std::vector<int>{2, 3} };

        auto x = mconcat(l);

        Assert(x == std::vector<int>{1,2,3});

    })

    .Single("monoid_string", []
    {
        Assert( mempty<std::string>().empty() );
        Assert (mappend(mappend(std::string{"1"}, std::string{"2"}), std::string{"3"}) == "123");
        Assert (mconcat(std::vector<std::string>{"1", "2","3"}) == "123");
    })


    .Single("monoid_list", []
    {
        Assert( mempty<std::list<int>>() == std::list<int>{} );

        auto app = mappend(std::list<int> {1}, std::list<int>{2}) ;
        Assert (app == std::list<int>{1, 2});

        auto l = std::list<std::list<int>>{ std::list<int>{}, std::list<int>{1}, std::list<int>{2, 3} };

        Assert (mconcat(l) == std::list<int>{1,2,3});
    })


    .Single("monoid_forward_list", []
    {
        Assert( mempty<std::forward_list<int>>() == std::forward_list<int>{} );

        auto app = mappend(std::forward_list<int> {1}, std::forward_list<int>{2}) ;
        Assert (app == std::forward_list<int>{1, 2});

        auto l = std::forward_list<std::forward_list<int>>{ std::forward_list<int>{}, std::forward_list<int>{1, 2}, std::forward_list<int>{3, 4, 5} };

        Assert (mconcat(l) == std::forward_list<int>{1,2,3,4,5});
    })


    .Single("monoid_deque", []
    {
        Assert( mempty<std::deque<int>>() == std::deque<int>{} );

        auto app = mappend(std::deque<int> {1}, std::deque<int>{2}) ;
        Assert (app == std::deque<int>{1, 2});

        auto l = std::deque<std::deque<int>>{ std::deque<int>{}, std::deque<int>{1}, std::deque<int>{2, 3} };

        Assert (mconcat(l) == std::deque<int>{1,2,3});
    })

    .Single("monoid_optional", []
    {
        auto a = Any{ true };
        auto b = Any{ false };

        Assert(mempty<optional<Any>>() == optional<Any>());

        Assert(mappend(make_optional(a), optional<Any>()) == a);
        Assert(mappend(optional<Any>(),  make_optional(b)) == b);
        Assert(mappend(make_optional(a), make_optional(b)) == mappend(a,b));
    })


    .Single("monoid_future", []
    {
        auto a = Any{ true };
        auto b = Any{ false };

        auto a_ = std::async([=]() { return a; });
        auto b_ = std::async([=]() { return b; });

        Assert(mempty<std::future<Any>>().get() == mempty<Any>());
        Assert(mappend(std::move(a_), std::move(b_)).get() == mappend(a, b));
    })


    .Single("monoid_map", []
    {
        Assert(mempty<std::map<int,std::string>>().empty());

        auto m1 = std::map<int, std::string>{ {0, "zero"} };
        auto m2 = std::map<int, std::string>{ {1, "one"} };

        auto m3 = mappend(m1, m2);

        Assert(m3 == std::map<int, std::string>{ {0, "zero"}, {1, "one"} } );
    })


    .Single("monoid_Any", []
    {
        Assert( mempty<Any>() == Any{ false });
        Assert( !mempty<Any>() );

        Assert( static_cast<bool>(mappend(Any{false}, Any{true})) );
        Assert( static_cast<bool>(mappend(Any{false}, Any{true})) );

        Assert(  static_cast<bool>(mconcat( std::vector<Any>{ Any{true}, Any{false}, Any{false} })) );
        Assert( !static_cast<bool>(mconcat( std::vector<Any>{ Any{false}, Any{false}, Any{false} })) );
    })


    .Single("monoid_All", []
    {
        Assert( mempty<All>() == All{ true });
        Assert( static_cast<bool>(mempty<All>()) );

        Assert( !static_cast<bool>(mappend(All{false}, All{true})) );
        Assert( !static_cast<bool>(mappend(All{false}, All{true})) );
        Assert( static_cast<bool>(mappend(All{true}, All{true})) );

        Assert( !static_cast<bool>(mconcat( std::vector<All>{ All{true}, All{false}, All{false} })) );
        Assert(  static_cast<bool>(mconcat( std::vector<All>{ All{true}, All{true}, All{true} })) );
    })


    .Single("monoid_Sum", []
    {
        Assert(mempty<Sum<int>>().value == 0);
        Assert(mappend(sum(1), sum(2)).value == 3);
        Assert(mconcat( std::vector<Sum<int>>{ sum(1), sum(2), sum(3) }).value == 6);
    })


    .Single("monoid_Product", []
    {
        Assert(mempty<Product<int>>().value == 1);
        Assert(mappend(product(1), product(2)).value == 2);
        Assert(mconcat( std::vector<Product<int>>{ product(1), product(2), product(3) }).value == 6);
    });

int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}
