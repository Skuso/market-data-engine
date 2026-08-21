#include <format>
#include <iostream>
#include <boost/beast.hpp>

int main() {

    std::cout << std::format("Hello, {}!\n", "World");
    
    return 0;
}
