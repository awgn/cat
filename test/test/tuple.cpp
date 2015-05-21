#include <iostream>

#include <cat/tuple.hpp>

#include <yats.hpp>

using namespace yats;
using namespace cat;

Context(tuple)
{
    Test(apply)
    {
        auto add = [](int a, int b) { return a+b; };

        std::tuple<int, int> tup{40,2};

        Assert(tuple_apply(add, tup), is_equal_to(42));
    }

    Test(apply_const)
    {
        auto add = [](int a, int b) { return a+b; };

        std::tuple<int, int> const tup{40,2};

        Assert(tuple_apply(add, tup), is_equal_to(42));
    }

    Test(apply_forward)
    {
        auto mistery = [](int &a, int b) -> int {
                a = 10;
            return a+b; };

        int a = 40;

        Assert(tuple_apply(mistery, std::forward_as_tuple(a,2)), is_equal_to(12));
        Assert(a, is_equal_to(10));
    }


    template <typename T>
    void test(T &v)
    {
        long int sum = 0;
        tuple_foreach([&](auto n) { sum += n; }, v);
    }

    Test(for_each)
    {
        std::tuple<int,int,int> v {1,2,3};
        std::tuple<long, long> q {1,2};

        test(v);
        test(q);
    }

    Test(for_each_const)
    {
        std::tuple<int,int,int> const v {1,2,3};
        int sum = 0;

        tuple_foreach([&](int n) {
                              sum += n;
                             }, v);

        Assert(sum, is_equal_to(6));
    }

    Test(for_each_forward)
    {
        int a = 1, b = 2, c = 3;
        int sum = 0;

        tuple_foreach([&](int n) {
                              sum += n;
                             }, std::forward_as_tuple(a,b,c));

        Assert(sum, is_equal_to(6));
    }

    Test(for_each_index)
    {
        std::tuple<int,int,int> const v {1,2,3};
        int sum = 0;
        int idx = 0;

        tuple_foreach_index([&](auto index, int n) {
                              sum += n;
                              idx += index.value;
                             }, v);

        Assert(sum, is_equal_to(6));
        Assert(idx, is_equal_to(3));
    }


    Test(map)
    {
        auto t = tuple_map(strlen, std::make_tuple("hello", "world"));

        size_t len = 0;

        tuple_foreach([&len](size_t n) { len+=n; }, t);

        Assert(len, is_equal_to(size_t(10)));
    }

    Test(map_const)
    {
        auto const t = std::make_tuple("hello", "world");

        auto ts = tuple_map(strlen, t);

        size_t len = 0;

        tuple_foreach([&len](size_t n) { len+=n; }, ts);

        Assert(len, is_equal_to(size_t(10)));
    }

    namespace noncopyable
    {
        struct type
        {
            type() = default;
            type(type &&) = default;
            type(const type &) = delete;
        };
    }


    Test(non_copyable)
    {
        auto t = std::make_tuple(noncopyable::type(), noncopyable::type());
        tuple_foreach([](auto) { }, std::move(t));
    }

    Test(assign)
    {
        std::tuple<int, char, std::string> a {1, 'x', "hello"};

        tuple_assign(a, 1, 'x');
        Assert(a == std::make_tuple(1, 'x', "hello"));

        tuple_assign_from<1>(a, 'y', "world!");
        Assert(a == std::make_tuple(1, 'y', "world!"));
    }


    Test(make_tuple)
    {
        auto t1 = cat::make_tuple<int, double>();
        Assert (t1 == std::make_tuple(0,0.0));

        auto t2 = cat::make_tuple<int, double>(42);
        Assert (t2 == std::make_tuple(42,0.0));

        auto t3 = cat::make_tuple<int, double>(42, 3.14);
        Assert (t3 == std::make_tuple(42, 3.14));
    }
}


int
main(int argc, char *argv[])
{
    return yats::run(argc, argv);
}
