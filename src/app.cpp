#include <iostream>
#include <string>
#include <vector>


std::vector<std::string> createVector() {
    return {"Hello", "World", "CI", "Docker"};
}

int main()
{
    auto testVector = createVector();
    std::cout << "Test CI in docker " << std::endl;
    return 0;
}

