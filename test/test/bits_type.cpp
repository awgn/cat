#include <cat/bits/type.hpp>

#include <yats.hpp>

using namespace yats;
using namespace cat;

Context(traits)
{
    Test(type_name)
    {
        Assert( cat::type_name<int>(), is_equal_to(std::string("int")));
        Assert( cat::type_name<const int>(), is_equal_to(std::string("int const")));
        Assert( cat::type_name<volatile int>(), is_equal_to(std::string("int volatile")));

        Assert( cat::type_name<int *>(), is_equal_to(std::string("int*")));
        Assert( cat::type_name<const int *>(), is_equal_to(std::string("int const*")));
        Assert( cat::type_name<int * const>(), is_equal_to(std::string("int* const")));
        Assert( cat::type_name<const int * const>(), is_equal_to(std::string("int const* const")));

        Assert( cat::type_name<int &>(), is_equal_to(std::string("int&")));
        Assert( cat::type_name<int &&>(), is_equal_to(std::string("int&&")));
        Assert( cat::type_name<int const &>(), is_equal_to(std::string("int const&")));
        Assert( cat::type_name<int const &&>(), is_equal_to(std::string("int const&&")));

        Assert( cat::type_name<int **>(), is_equal_to(std::string("int**")));
        Assert( cat::type_name<const int **>(), is_equal_to(std::string("int const**")));
        Assert( cat::type_name<int ** const>(), is_equal_to(std::string("int** const")));
        Assert( cat::type_name<const int ** const>(), is_equal_to(std::string("int const** const")));

        Assert( cat::type_name<int * const *>(), is_equal_to(std::string("int* const*")));
        Assert( cat::type_name<int const * const * >(), is_equal_to(std::string("int const* const*")));
    }

    Test(type_of)
    {
        int n;
        int & ref = n;
        int const & ref_const = n;

        int * p;
        int * & p_ref = p;

        Assert( cat::type_of(n), is_equal_to(std::string("int&")));
        Assert( cat::type_of(ref), is_equal_to(std::string("int&")));
        Assert( cat::type_of(ref_const), is_equal_to(std::string("int const&")));

        Assert( cat::type_of(std::move(n)), is_equal_to(std::string("int&&")));
        Assert( cat::type_of(std::move(ref)), is_equal_to(std::string("int&&")));
        Assert( cat::type_of(std::move(ref_const)), is_equal_to(std::string("int const&&")));

        Assert( cat::type_of(p), is_equal_to(std::string("int*&")));
        Assert( cat::type_of(p_ref), is_equal_to(std::string("int*&")));

        Assert( cat::type_of(std::move(p)), is_equal_to(std::string("int*&&")));
        Assert( cat::type_of(std::move(p_ref)), is_equal_to(std::string("int*&&")));
    }
}


int
main(int argc, char * argv[])
{
    return yats::run(argc, argv);
}


