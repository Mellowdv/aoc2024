#include <bits/stdc++.h>

bool recursive_backtrack(std::vector<int64_t> const &components,
                             int64_t const goal,
                             int64_t current_sum,
                             std::size_t current_idx) {
    if (current_sum > goal) return false;
    if (current_idx == components.size()) {
        if (current_sum == goal) {
            return true;
        } else {
            return false;
        }
    }
    int64_t concat_result
        {std::stoll({std::to_string(current_sum) + std::to_string(components[current_idx])})};
    // Try each operator
    return recursive_backtrack(components, goal, current_sum + components[current_idx], current_idx + 1) ||
           recursive_backtrack(components, goal, current_sum * components[current_idx], current_idx + 1) ||
           recursive_backtrack(components, goal, concat_result, current_idx + 1);
};

int main() {
    int64_t target;
    std::vector<int64_t> components;

    std::string l;
    int64_t tmp;
    int64_t total_sum {0};
    int64_t total_sum_p2 {0};
    uint64_t absolute_max {0};
    while (std::getline(std::cin, l)) {
        components.clear();
        std::stringstream ss {l};
        ss >> target;
        ss >> l;
        while (ss >> tmp) {
            components.push_back(tmp);
        }

        uint64_t mask_max = 1 << (components.size() - 1);
        absolute_max = std::max(mask_max, absolute_max);
        int64_t current {0};
        for (int mask = 0; mask < mask_max; mask++) {
            current = components[0];
            int64_t component_index = 1;
            for (int j = 0; j < components.size() - 1; j++) {
                if ((1ULL << j) & mask) {
                    current += components[component_index];
                } else {
                    current *= components[component_index];
                }
                component_index++;
            }
            if (current > target) continue;
            if (current == target) {
                total_sum += target;
                break;
            }
        }
        if (recursive_backtrack(components, target, components[0], 1)) {
            total_sum_p2 += target;
        }
    }
    std::cout << total_sum << "\n";
    std::cout << total_sum_p2 << "\n";
}
