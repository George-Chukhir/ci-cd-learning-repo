#include "include/app.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <memory>

int main() {
    auto app_obj = std::make_unique<App>();

    auto vec = app_obj->createVector();
    for (const auto& str : vec) {
        std::cout << str << std::endl;
    }
    return 0;
}