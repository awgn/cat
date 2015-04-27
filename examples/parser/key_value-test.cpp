/* ----------------------------------------------------------------------------
 *
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <nicola@pfq.io> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return. Nicola Bonelli
 * ----------------------------------------------------------------------------
 */

#include <key_value.hpp>

#include <list>
#include <string>
#include <set>
#include <map>
#include <memory>


namespace key
{
    DECLARE_KEY( unsigned_int );
    DECLARE_KEY( integers );
    DECLARE_KEY( booleans );
    DECLARE_KEY( strings );
    DECLARE_KEY( intset );

    DECLARE_KEY( associative );
    DECLARE_KEY( simple_pair );
    DECLARE_KEY( simple_tuple );

    DECLARE_KEY( optional_int );
    DECLARE_KEY( int_matrix );

    DECLARE_KEY( inline_doc );

    DECLARE_KEY( first );
    DECLARE_KEY( second );
}


using namespace parser;

using pair_doc =
    document
    <
        key_value<key::first,  int>,
        key_value<key::second, int>
    >;


using this_document =
    document
    <
        key_value<key::unsigned_int, unsigned int>,
        key_value<key::integers,     std::vector<int>>,
        key_value<key::booleans,     std::vector<bool>>,
        key_value<key::strings,      std::list<std::string>>,
        key_value<key::simple_pair,  std::pair<double, double>>,
        key_value<key::simple_tuple, std::tuple<bool, int, double, std::string>>,
        key_value<key::associative,  std::map<std::string, int>>,
        key_value<key::intset,       std::set<int>>,
        key_value<key::optional_int, cat::optional<int>>,
        key_value<key::int_matrix,   std::vector<std::vector<int>>>,
        key_value<key::inline_doc,   std::vector<pair_doc>>
    >;


int
main(int, char *[])
{
    auto str  = parser::read_file("./config.txt");
    auto str_ = parser::remove_comments(str, '#');
    auto doc  = read<this_document>(str_);

    std::cout << "document       :" << show(doc) << std::endl;
    std::cout << "unsigned_int   : " << show( doc.get<key::associative>() ) << std::endl;
    std::cout << "integers vector: " << show( doc.get<key::integers>() ) << std::endl;
    std::cout << "booleans       : " << show( doc.get<key::booleans>() ) << std::endl;
    std::cout << "strings        : " << show( doc.get<key::strings>() ) << std::endl;

    return 0;
}

