#include "../src/smalltoolbox_text.hpp"

#include <cassert>
#include <iostream>


auto main() -> int
{
    using namespace stbox;

    assert(Text::join(std::vector<std::string> {"Hello", "World!"}, char(32)) == "Hello World!");
    assert((Text::split("Hello World!", char(32)) == std::vector<std::string> {"Hello", "World!"}));
    assert((Text::trim(std::vector<std::string> {"100", "001"}, '0') == std::vector <std::string> {"1", "1"}));
    assert(Text::replace("010110", '1', "--") == "0--0----0");
    assert(Text::trim("0.100", '0') == ".1");
    assert(Text::ltrim("0001", '0') == "1");
    assert(Text::rtrim("1000", '0') == "1");
    assert(Text::trimZeros(1000) == "1000.0");
    assert(Text::trimZeros(1000.50) == "1000.50");
    assert(Text::strUpper("help") == "HELP");
    assert(Text::strLower("HelP") == "help");

    std::cout << "Text test finished!\n";

    return 0;
}
