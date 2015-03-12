#include <cat/container.hpp>

#include <yats.hpp>

using namespace cat;
using namespace yats;


Context(test_container)
{
    Test(vector)
    {
        std::vector<int> v {1};

        insert(v, 2);
        Assert(v, is_equal_to(std::vector<int>{1, 2}));

        insert(v, {3, 4, 5});
        Assert(v, is_equal_to(std::vector<int>{1, 2, 3, 4, 5}));
    }

    Test(list)
    {
        std::list<int> v {1};

        insert(v, 2);
        Assert(v, is_equal_to(std::list<int>{1, 2}));

        insert(v, {3, 4, 5});
        Assert(v, is_equal_to(std::list<int>{1, 2, 3, 4, 5}));
    }

    Test(deque)
    {
        std::deque<int> v {1};

        insert(v, 2);
        Assert(v, is_equal_to(std::deque<int>{1, 2}));

        insert(v, {3, 4, 5});
        Assert(v, is_equal_to(std::deque<int>{1, 2, 3, 4, 5}));
    }

    Test(forward_list)
    {
        std::forward_list<int> v {1};

        insert(v, 2);
        Assert(v, is_equal_to(std::forward_list<int>{1, 2}));

        insert(v, {3, 4, 5});
        Assert(v, is_equal_to(std::forward_list<int>{1, 2, 3, 4, 5}));
    }


    Test(set)
    {
        std::set<int> v {1};

        insert(v, 2);
        Assert(v, is_equal_to(std::set<int>{1, 2}));

        insert(v, {3, 4, 5});
        Assert(v, is_equal_to(std::set<int>{1, 2, 3, 4, 5}));

    }

    Test(map)
    {
        std::map<int, std::string> v {{1, "hello"}};

        insert(v, std::make_pair(2, "world"));
        Assert(v, is_equal_to(std::map<int, std::string>{{1, "hello"},{2, "world"}}));
    }
}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

