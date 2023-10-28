#include "../src/smalltoolbox_system.hpp"

#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>


auto main() -> int
{
    stbox::Sys::Console::view(std::vector<std::string> {"A", "B", "C"});
    stbox::Sys::Console::view(std::vector<double> {1.1, 2.2, 3.3});

    std::cout << "System test finished!\n";

    return 0;
}
