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


        auto l = std::list<std::vector<int>>{ std::vector<int>{}, std::vector<int>{1}, std::vector<int>{2} };

        Assert ((mconcat(l) == std::vector<int>{1,2}));
    }

}


int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}
