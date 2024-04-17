#include "chrono.hpp"
#include <iostream>

int hoge(int a)
{
    return a + 100;
}

int main()
{

    auto result = MeasureTime(hoge, 1);
    // auto result = MeasureTime(std::function<int(int)>(hoge), 1);
    std::cout << result.first << std::endl;
    std::cout << result.second << std::endl;

    return 0;
}