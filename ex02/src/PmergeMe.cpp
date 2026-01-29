#include "PmergeMe.hpp"
#include <algorithm>
#include <iterator>
#include <vector>
int PmergeMe::_comparisons = 0;

void PmergeMe::ford_johnson_sort(std::vector<int> &data) noexcept
{
    merge_insertion_sort(data, 1);
}

void PmergeMe::ford_johnson_sort(std::deque<int> &data) noexcept
{
    merge_insertion_sort(data, 1);
}

int PmergeMe::jacobsthal_nr(int n) noexcept
{
    // (2^(n+1) + (-1)^n) / 3
    // 2^(n+1) is the same as 1 << (n+1)
    int second_term = (n % 2 == 0) ? 1 : -1;
    return ((1 << (n + 1)) + second_term) / 3;
}

template <typename T> T PmergeMe::move(T iter, int amount) noexcept
{
    std::advance(iter, amount);
    return iter;
}

template <typename Ival, typename T> bool PmergeMe::compare(Ival lva, T rva) noexcept
{
    PmergeMe::_comparisons++;
    return *lva < *rva;
}

template <typename T>
void PmergeMe::binary_insertion_sort(
    std::vector<typename T::iterator> &main, // NOLINT(bugprone-easily-swappable-parameters)
    std::vector<typename T::iterator> &pend) noexcept
{
    auto pend_val = pend.begin();
    auto insert_pos = std::upper_bound(main.begin(), main.end(), **pend_val,
                                       [](const int val, typename T::iterator mc_it) { return compare(&val, mc_it); });
    main.insert(insert_pos, *pend_val);
    pend.erase(pend_val);
}

template <typename T>
void PmergeMe::move_back(std::vector<typename T::iterator> &main_chain, T &container, int group_size) noexcept
{
    std::vector<int> copy;
    copy.reserve(container.size());
    for (auto it = main_chain.begin(); it != main_chain.end(); it++)
    {
        for (int i = 0; i < group_size; i++)
        {
            typename T::iterator pair_start = *it;
            std::advance(pair_start, -group_size + i + 1); // move to start of the pair
            copy.insert(copy.end(), *pair_start);
        }
    }

    /* Replace values in the original container. */
    std::copy(copy.begin(), copy.end(), container.begin());
}

template <typename T> void PmergeMe::merge_insertion_sort(T &container, int group_size) noexcept
{
    using c_iter = typename T::iterator;

    int n_groups = container.size() / group_size;
    if (n_groups < 2)
    {
        return;
    }

    bool has_odd = n_groups % 2 != 0;

    c_iter start = container.begin();
    c_iter end = container.end();
    c_iter bound = has_odd ? move(start, (n_groups - 1) * group_size) : end;

    for (c_iter it = start; it + (group_size * 2) <= bound; std::advance(it, group_size * 2))
    {
        auto l_val = *(it + group_size - 1);
        auto r_val = *(it + (group_size * 2) - 1);
        if (l_val > r_val)
        {
            std::swap_ranges(it, it + group_size, it + group_size);
        }
    }

    // print_container_in_groups(container, group_size);
    merge_insertion_sort(container, group_size * 2);

    // setup main and pend chain.
    std::vector<c_iter> main_chain; // basically keeping pointers to the max of
                                    // each group (main chain a1, b1, a2, ax, ...)
    std::vector<c_iter> pend_chain; // keeping pointers to the min of each
                                    // group (pend chain b2, bx, ...)

    main_chain.emplace_back(move(container.begin(), group_size - 1));       // insert B1
    main_chain.emplace_back(move(container.begin(), (group_size * 2) - 1)); // insert A1

    int starting_group = 4; // we already inserted groups 1 and 2
    // setup chains
    for (int i = starting_group; i <= n_groups; i += 2)
    {
        pend_chain.emplace_back(move(container.begin(), (group_size * (i - 1)) - 1));
        main_chain.emplace_back(move(container.begin(), (group_size * i) - 1));
    }

    /**
     * We have an odd number of groups, so the last (full) group only has to go
     * to the pend chain
     */
    if (has_odd)
    {
        pend_chain.emplace_back(move(container.begin(), ((group_size * (n_groups)) - 1)));
    }

    int previous_jacobsthal = jacobsthal_nr(1);
    for (int i = 2; !pend_chain.empty(); i++) // we start at Jacobsthal 2 (which is 3, because 1 and 2 are already
                                              // inserted in the main chain)
    {
        int current_jacobsthal = jacobsthal_nr(i);
        int to_insert = current_jacobsthal - previous_jacobsthal;

        if (to_insert > static_cast<int>(pend_chain.size()))
        {
            break;
        }

        for (int j = 0; j < to_insert; j++)
        {
            binary_insertion_sort<T>(main_chain, pend_chain);
        }
        previous_jacobsthal = current_jacobsthal;
    }

    // final insertion for any remaining pend elements (just have to do this in
    // reverse order since we're pointering to the back of the pend chain)
    for (unsigned int i = 0; i < pend_chain.size(); i++)
    {
        auto pend_val = move(pend_chain.begin(), i);
        auto insert_pos = std::upper_bound(main_chain.begin(), main_chain.end(), **pend_val,
                                           [](const int val, c_iter mc_it) { return compare(&val, mc_it); });
        main_chain.insert(insert_pos, *pend_val);
    }

    // move back to original container
    move_back<T>(main_chain, container, group_size);
}