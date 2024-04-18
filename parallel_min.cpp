#include <iostream>
#include <vector>
#include <ranges>
#include <thread>

int main()
{
    const auto input_ranges = std::vector<std::int32_t>{1, 2, 3, 4, 5, 6, 6, 7, 7, 4, 3, 2, 34, 35, 234, -2, 6, 5, 4, 645, 456, 45, 6};

    const std::size_t num_threads = std::thread::hardware_concurrency();
    auto threads = std::vector<std::thread>(num_threads);
    auto min_per_thread = std::vector<std::int32_t>(num_threads);

    auto get_min_range = [&min_per_thread](auto vec, std::size_t tid)
    {
        min_per_thread[tid] = *std::ranges::min_element(vec);
    };

    auto cumul_offset = std::size_t{0};
    for (auto tid = std::size_t{0}; tid < num_threads; ++tid)
    {
        const auto assigned_size = (tid < input_ranges.size() % num_threads) ? (input_ranges.size() + num_threads - 1) / num_threads : input_ranges.size() / num_threads;
        threads[tid] = std::thread(get_min_range, std::ranges::subrange(input_ranges.begin() + cumul_offset, input_ranges.begin() + cumul_offset + assigned_size), tid);
        cumul_offset += assigned_size;
    }

    for (auto tid = std::size_t{0}; tid < num_threads; ++tid)
    {
        threads[tid].join();
    }
    auto min_value = *std::ranges::min_element(min_per_thread);
    std::cout << min_value << std::endl;

    return 0;
}