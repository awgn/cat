#include <cat/functor.hpp>

#include <yats.hpp>

using namespace yats;
using namespace cat;


Context(functor)
{

    template <template <typename...> class F, typename ...Ts>
    void functor_constraint(F<Ts...> const &)
    {
        static_assert(is_functor<F>::value, "F: not a functor!");
    }


    template <template <typename ...> class F, typename T>
    auto make_uniques(std::initializer_list<T> xs)
    {
        F<std::unique_ptr<T>> out;
        for(auto &x : xs)
            out.push_back(std::make_unique<T>(std::move(x)));
        return out;
    }
    template <template <typename ...> class F, typename T>
    auto make_uniques_rev(std::initializer_list<T> xs)
    {
        F<std::unique_ptr<T>> out;
        for(auto &x : xs)
            out.push_front(std::make_unique<T>(std::move(x)));
        return out;
    }


    Test(functor_vector)
    {
        std::vector<std::string> vec { "one", "two", "three" };
        auto r = fmap([](const std::string &s) -> size_t { return s.size(); }, vec);
        Assert(r, is_equal_to(std::vector<size_t>{3, 3, 5}));


        auto vec2 = make_uniques<std::vector>({1,2,3});

        auto r1 = fmap([](const std::unique_ptr<int> &p) -> int { return *p; }, vec2 );
        auto r2 = fmap([](std::unique_ptr<int> p) -> int { return *p; }, std::move(vec2) );

        Assert(r1, is_equal_to( std::vector<int>{1,2,3} ));
        Assert(r2, is_equal_to( std::vector<int>{1,2,3} ));
    }


    Test(functor_deque)
    {
        std::deque<std::string> vec { "one", "two", "three" };
        auto r = fmap([](const std::string &s) -> size_t { return s.size(); }, vec);
        Assert(r, is_equal_to(std::deque<size_t>{3, 3, 5}));


        auto vec2 = make_uniques<std::deque>({1,2,3});

        auto r1 = fmap([](const std::unique_ptr<int> &p) -> int { return *p; }, vec2 );
        auto r2 = fmap([](std::unique_ptr<int> p) -> int { return *p; }, std::move(vec2) );

        Assert(r1, is_equal_to( std::deque<int>{1,2,3} ));
        Assert(r2, is_equal_to( std::deque<int>{1,2,3} ));
    }


    Test(functor_list)
    {
        std::list<std::string> vec { "one", "two", "three" };
        auto r = fmap([](const std::string &s) -> size_t { return s.size(); }, vec);
        Assert(r, is_equal_to(std::list<size_t>{3, 3, 5}));

        auto vec2 = make_uniques<std::list>({1,2,3});

        auto r1 = fmap([](const std::unique_ptr<int> &p) -> int { return *p; }, vec2 );
        auto r2 = fmap([](std::unique_ptr<int> p) -> int { return *p; }, std::move(vec2) );

        Assert(r1, is_equal_to( std::list<int>{1,2,3} ));
        Assert(r2, is_equal_to( std::list<int>{1,2,3} ));
    }


    Test(functor_forward_list)
    {
        std::forward_list<std::string> vec { "one", "two", "three" };
        auto r = fmap([](const std::string &s) -> size_t { return s.size(); }, vec);
        Assert(r, is_equal_to(std::forward_list<size_t>{3, 3, 5}));


        auto vec2 = make_uniques_rev<std::forward_list>({3,2,1});

        auto r1 = fmap([](const std::unique_ptr<int> &p) -> int { return *p; }, vec2 );
        auto r2 = fmap([](std::unique_ptr<int> p) -> int { return *p; }, std::move(vec2) );

        Assert(r1, is_equal_to( std::forward_list<int>{1,2,3} ));
        Assert(r2, is_equal_to( std::forward_list<int>{1,2,3} ));
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

        std::map<int, std::unique_ptr<std::string>> m2;
        m2.insert(std::make_pair(1, std::make_unique<std::string>("one") ));
        m2.insert(std::make_pair(2, std::make_unique<std::string>("two") ));
        m2.insert(std::make_pair(3, std::make_unique<std::string>("three") ));

        auto r2 = fmap([](const std::unique_ptr<std::string> &s) { return s->size(); }, m2);
        Assert(r2, is_equal_to(std::map<int,size_t>{{1,3}, {2,3}, {3,5}}));

        auto r3 = fmap([](std::unique_ptr<std::string> s) { return s->size(); }, std::move(m2));

        Assert(r3, is_equal_to(std::map<int,size_t>{{1,3}, {2,3}, {3,5}}));
    }


    Test(functor_multimap)
    {
        std::multimap<int, std::string> m { {1,"one"}, {2,"two"}, {3,"three"} };
        auto r = fmap([](const std::string &s) { return s.size(); }, m);
        Assert(r, is_equal_to(std::multimap<int,size_t>{{1,3}, {2,3}, {3,5}}));

        std::multimap<int, std::unique_ptr<std::string>> m2;
        m2.insert(std::make_pair(1, std::make_unique<std::string>("one") ));
        m2.insert(std::make_pair(2, std::make_unique<std::string>("two") ));
        m2.insert(std::make_pair(3, std::make_unique<std::string>("three") ));

        auto r2 = fmap([](const std::unique_ptr<std::string> &s) { return s->size(); }, m2);
        Assert(r2, is_equal_to(std::multimap<int,size_t>{{1,3}, {2,3}, {3,5}}));

        auto r3 = fmap([](std::unique_ptr<std::string> s) { return s->size(); }, std::move(m2));

        Assert(r3, is_equal_to(std::multimap<int,size_t>{{1,3}, {2,3}, {3,5}}));
    }


    Test(functor_unordered_map)
    {
        std::unordered_map<int, std::string> m { {1,"one"}, {2,"two"}, {3,"three"} };
        auto r = fmap([](const std::string &s) { return s.size(); }, m);
        Assert(r, is_equal_to(std::unordered_map<int,size_t>{{1,3}, {2,3}, {3,5}}));

        std::unordered_map<int, std::unique_ptr<std::string>> m2;
        m2.insert(std::make_pair(1, std::make_unique<std::string>("one") ));
        m2.insert(std::make_pair(2, std::make_unique<std::string>("two") ));
        m2.insert(std::make_pair(3, std::make_unique<std::string>("three") ));

        auto r2 = fmap([](const std::unique_ptr<std::string> &s) { return s->size(); }, m2);
        Assert(r2, is_equal_to(std::unordered_map<int,size_t>{{1,3}, {2,3}, {3,5}}));

        auto r3 = fmap([](std::unique_ptr<std::string> s) { return s->size(); }, std::move(m2));

        Assert(r3, is_equal_to(std::unordered_map<int,size_t>{{1,3}, {2,3}, {3,5}}));
    }


    Test(functor_unordered_multimap)
    {
        std::unordered_multimap<int, std::string> m { {1,"one"}, {2,"two"}, {3,"three"} };
        auto r = fmap([](const std::string &s) { return s.size(); }, m);
        Assert(r, is_equal_to(std::unordered_multimap<int,size_t>{{1,3}, {2,3}, {3,5}}));

        std::unordered_multimap<int, std::unique_ptr<std::string>> m2;
        m2.insert(std::make_pair(1, std::make_unique<std::string>("one") ));
        m2.insert(std::make_pair(2, std::make_unique<std::string>("two") ));
        m2.insert(std::make_pair(3, std::make_unique<std::string>("three") ));

        auto r2 = fmap([](const std::unique_ptr<std::string> &s) { return s->size(); }, m2);
        Assert(r2, is_equal_to(std::unordered_multimap<int,size_t>{{1,3}, {2,3}, {3,5}}));

        auto r3 = fmap([](std::unique_ptr<std::string> s) { return s->size(); }, std::move(m2));

        Assert(r3, is_equal_to(std::unordered_multimap<int,size_t>{{1,3}, {2,3}, {3,5}}));
    }


    Test(functor_pair)
    {
        auto p = std::make_pair(std::string("one"), std::string("two"));

        auto r = fmap([](const std::string &s) -> size_t { return s.size(); }, p);
        Assert(r, is_equal_to(std::pair<std::string, size_t>{"one", 3}));

        auto p2 = std::make_pair(std::string("one"), std::make_unique<std::string>("two"));

        auto r2 = fmap([](const std::unique_ptr<std::string> &s) -> size_t { return s->size(); }, p2);
        Assert(r2, is_equal_to(std::pair<std::string, size_t>{"one", 3}));

        auto r3 = fmap([](std::unique_ptr<std::string> s) -> size_t { return s->size(); }, std::move(p2));
        Assert(r2, is_equal_to(std::pair<std::string, size_t>{"one", 3}));
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
        functor_constraint( std::multimap<std::string, int>{} );
        functor_constraint( std::unordered_multimap<std::string, int>{} );
    }

}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

