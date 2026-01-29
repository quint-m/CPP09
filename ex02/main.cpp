#include "PmergeMe.hpp"

#include <chrono>
#include <deque>
#include <iostream>
#include <ostream>
#include <ratio>
#include <sstream>
#include <vector>

template <typename T> std::ostringstream print_container(const T &container)
{
    std::ostringstream out;
    for (auto it = container.begin(); it != container.end(); ++it)
    {
        out << *it << " ";
    }
    return out;
}

std::chrono::duration<double, std::micro> benchmark_vec(std::vector<int> &data)
{
    auto start = std::chrono::high_resolution_clock::now();
    PmergeMe::ford_johnson_sort(data);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::micro> duration = end - start;
    return duration;
}

std::chrono::duration<double, std::micro> benchmark_deq(std::deque<int> &data)
{
    auto start = std::chrono::high_resolution_clock::now();
    PmergeMe::ford_johnson_sort(data);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::micro> duration = end - start;
    return duration;
}

int parseArgv(char **argv, int index)
{
    try
    {
        long value = std::stol(argv[index]); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (value < 0)
        {
            throw std::runtime_error(
                "Negative value: " +
                std::string(argv[index])); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        }
        if (value > std::numeric_limits<int>::max())
        {
            throw std::runtime_error(
                "Value exceeds int max: " +
                std::string(argv[index])); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        }
        return static_cast<int>(value);
    }
    catch (const std::invalid_argument &)
    {
        throw std::runtime_error("Invalid argument: " +
                                 std::string(argv[index])); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }
    catch (const std::out_of_range &)
    {
        throw std::runtime_error("Argument out of range: " +
                                 std::string(argv[index])); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <string>\n"; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        return 1;
    }

    try
    {
        std::vector<int> vec_data;
        std::deque<int> deq_data;
        for (int i = 1; i < argc; i++)
        {
            int value = parseArgv(argv, i);
            vec_data.push_back(value);
            deq_data.push_back(value);
        }

        std::cout << "Before: [" << print_container(vec_data).str() << "]\n";
        auto duration_vec = benchmark_vec(vec_data);
        auto duration_deq = benchmark_deq(deq_data);
        std::cout << "After: [" << print_container(vec_data).str() << "]\n";
        std::cout << "Number of comparisons: " << PmergeMe::_comparisons << "\n";
        std::cout << "Vector sort time: " << duration_vec.count() << " microseconds\n";
        std::cout << "Deque sort time: " << duration_deq.count() << " microseconds\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}