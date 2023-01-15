#include <iostream>

#include "../vec.h"

auto test() -> int {
    auto vec = Vec<int>::of(10);
    std::cout << vec << '\n';
    return 0;
}

auto main() -> int {
    return test();
}
