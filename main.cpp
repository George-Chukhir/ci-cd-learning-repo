#include "app.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <chrono>
#include <thread>

int main() {
    auto app_obj = std::make_unique<App>();

    auto vec = app_obj->createVector();
    for (const auto& str : vec) {
        std::cout << str << std::endl;
    }


    // Azure hack
    // while (true) {
    //     std::this_thread::sleep_for(std::chrono::hours(1));
    // }

    return 0;
}