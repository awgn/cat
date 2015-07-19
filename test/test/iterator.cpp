#include <cat/iterator.hpp>

#include <yats.hpp>

using namespace yats;
using namespace cat;

// Tests:
//

auto g = Group("iterator")
    .Single("map_iterator", []
    {
        std::vector<std::string> v { "one", "two", "three" };

        auto s = [](const std::string &e) { return e.size(); };

        std::vector<size_t> r { map_iterator( std::begin(v), s),
                                map_iterator( std::end(v), s) };

        Assert( r == std::vector<size_t> {3, 3, 5} );
    });

int
main(int argc, char*  argv[])
{
    return yats::run(argc, argv);
}

