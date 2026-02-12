
#ifndef ASSIGNMENT2_UTILS_HPP
#define ASSIGNMENT2_UTILS_HPP

#include <algorithm>
#include <numeric>
#include <set>
#include <vector>

namespace utils {
    class Invalid : std::exception {};

    template <typename T>
    bool contains_duplicate(const std::vector<T>& v) {
        return std::set<T>(v.begin(), v.end()).size() != v.size();
    }

    template <typename T>
    std::vector<size_t> sort_to_index(const std::vector<T>& v) {
        std::vector<size_t> idx(v.size());
        std::iota(idx.begin(), idx.end(), 0);
        std::stable_sort(idx.begin(), idx.end(), [&v](size_t i, size_t j) { return v[i] < v[j]; });
        return idx;
    }

    template <typename T>
    void sort_by_index(std::vector<T>& v, const std::vector<size_t>& idx) {
        if(v.size() != idx.size()) {
            throw Invalid();
        }
        const auto base = v.begin();
        for(size_t i = 0; i < idx.size(); i++) {
            if(idx[i] > i) {
                auto first = base;
                auto second = base;
                std::advance(first, i);
                std::advance(second, idx[i]);
                iter_swap(first, second);
            }
        }
    }
}

#endif //ASSIGNMENT2_UTILS_HPP
