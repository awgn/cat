#include <cat/section.hpp>

#include <yats.hpp>

using namespace yats;
using namespace cat;

Context(traits)
{
    Test(section_test)
    {

#if defined(__clang__) || (__GNUC__ >= 5)
        auto a = sec(10, minus<int>);
        auto b = sec(minus<int>, 10);
#else
        auto a = sec(10, std::minus<int>{});
        auto b = sec(std::minus<int>{}, 10);
#endif
        Assert( a(5), is_equal_to(5) );
        Assert( b(5), is_equal_to(-5) );

        Assert( sec([](int a, int b) { return a+b; }, 1)(41), is_equal_to(42) );
    }
}


int
main(int argc, char * argv[])
{
    return yats::run(argc, argv);
}


