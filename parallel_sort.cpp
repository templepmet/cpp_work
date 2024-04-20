#include <iostream>
#include <vector>
#include <ranges>
#include <thread>
#include <cassert>
#include <random>

template <std::ranges::random_access_range Range>
    requires std::sortable<std::ranges::iterator_t<Range>>
void sort_single(Range &&input_range)
{
    std::ranges::sort(input_range);
}

template <std::ranges::random_access_range Range>
    requires std::sortable<std::ranges::iterator_t<Range>>
void sort_parallel(Range &&input_range, std::size_t num_threads = std::thread::hardware_concurrency())
{
    const auto range_size = std::ranges::size(input_range);
    auto threads = std::vector<std::thread>(num_threads);

    std::vector<std::ranges::iterator_t<Range>> divided_iterators(num_threads + 1);
    divided_iterators[0] = std::ranges::begin(input_range);
    for (auto tid = std::size_t{0}; tid < num_threads; ++tid)
    {
        const auto assign_size = (tid < range_size % num_threads) ? (range_size + num_threads - 1) / num_threads : range_size / num_threads;
        divided_iterators[tid + 1] = divided_iterators[tid] + assign_size;
        threads[tid] = std::thread(std::ranges::sort, std::ranges::subrange(divided_iterators[tid], divided_iterators[tid + 1]));
    }
    for (auto tid = std::size_t{0}; tid < num_threads; ++tid)
    {
        threads[tid].join();
    }
    // (0,1,2) (2,3,4) (4,5,6) (6,7,8) // 4thread
    // (0+1,1+1) (2+1,3+1) 4+1
    for (auto tid = std::size_t{0}; tid < num_threads - 1; ++tid)
    {
        std::ranges::inplace_merge(std::ranges::subrange(divided_iterators[0], divided_iterators[tid + 2]), divided_iterators[tid + 1]);
    }
}

int main()
{
    const std::size_t vector_size = 10000000; // 1e6
    auto engine = std::mt19937(0);
    auto dist_value = std::uniform_int_distribution<>();

    auto range_single = std::vector<std::int32_t>(vector_size);
    for (auto &&value : range_single)
    {
        value = dist_value(engine);
    }
    auto range_parallel = range_single;

    auto t_single_begin = std::chrono::high_resolution_clock::now();
    sort_single(range_single);
    auto t_single_end = std::chrono::high_resolution_clock::now();
    auto duration_single = std::chrono::duration_cast<std::chrono::microseconds>(t_single_end - t_single_begin).count();
    std::cout << "duration_single[second]:" << duration_single / 1e6 << std::endl;

    auto t_parallel_begin = std::chrono::high_resolution_clock::now();
    sort_parallel(range_parallel, 2);
    auto t_parallel_end = std::chrono::high_resolution_clock::now();
    auto duration_parallel = std::chrono::duration_cast<std::chrono::microseconds>(t_parallel_end - t_parallel_begin).count();
    std::cout << "duration_parallel[second]:" << duration_parallel / 1e6 << std::endl;

    assert(range_single == range_parallel);

    return 0;
}