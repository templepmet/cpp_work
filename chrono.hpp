#pragma once

#include <utility>
#include <functional>

template <class... Args>
std::pair<int, double> MeasureTime(const std::function<int(Args...)> &func, const Args &...args)
{
    return {func(args...), 0.0};
}

// template <class... Args>
// std::pair<int, double> MeasureTime(const std::function<int(int)> &func, const Args &...args)
// {
//     return {func(args...), 1.2};
// }

// template <class Func, class... Args>
// std::pair<decltype(auto), double> MeasureTime(Func func, const Args &...args)
// {
//     return {func(args...), 0.0};
// }
