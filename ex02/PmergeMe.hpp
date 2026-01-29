#pragma once

#include <deque>
#include <vector>

class PmergeMe
{
  public:
    static void ford_johnson_sort(std::vector<int> &data) noexcept;

    static void ford_johnson_sort(std::deque<int> &data) noexcept;

    static int _comparisons;

  private:
    static int jacobsthal_nr(int n) noexcept;

    template <typename T>
    static void move_back(std::vector<typename T::iterator> &main_chain, T &container, int group_size) noexcept;

    template <typename T>
    static void binary_insertion_sort(std::vector<typename T::iterator> &main_chain,
                                      std::vector<typename T::iterator> &pend_chain) noexcept;

    template <typename T> static void merge_insertion_sort(T &container, int group_size) noexcept;

    template <typename Ival, typename T> static bool compare(Ival lva, T rva) noexcept;

    template <typename T> static T move(T iter, int amount) noexcept;
};