// Side note: I used int8_t and deeply regret it, any printing is a nightmare
// full of static_cast<int16_t>...
// C++20 required for std::views::split
#include <bits/stdc++.h>
static std::array<int8_t, 100> page_indices {};
int main() {
    std::unordered_map<int8_t, std::vector<int8_t>> page_to_ordering {};
    std::vector<int8_t> update {};
    std::vector<std::vector<int8_t>> incorrect_updates {};
    int8_t p;
    std::string l;

    // Read in constraints for each page
    while (std::getline(std::cin, l)) {
        if (l == "") break;
        std::vector<int8_t> tmp {};
        for (const auto w: std::views::split(l, '|')) {
            std::from_chars(w.data(), w.data() + w.size(), p);
            tmp.push_back(p); 
        }
        page_to_ordering[tmp[0]].push_back(tmp[1]);
    }

    int sum = 0;
    while (std::getline(std::cin, l)) {
        update.clear();
        page_indices.fill(101);

        int index = 0;
        std::from_chars_result res {.ptr = l.data() - 1};
        for (const auto w: std::views::split(l, ',')) {
            std::from_chars(w.data(), w.data() + w.size(), p);
            update.push_back(p);
            page_indices[p] = index;
            index++;
        }

        bool good = true;
        for (auto const &p: update) {
            auto const &orderings = page_to_ordering[p];
            int p_index = page_indices[p];
            for (auto const &o: orderings) {
                if (page_indices[o] < p_index) {
                    good = false;
                    break;
                }
            }
            if (!good) {
                incorrect_updates.push_back(update);
                break;
            }
        }
        if (good) {
            sum += update[(update.size() - 1) / 2];
        }
    }
    std::cout << sum << "\n";

    // Find the number in each incorrect update whose constraint has exactly
    // (incorrect_update.size() - 1) / 2 pages, then add it and break the inner loop
    // This will be the middle number
    // To see why - what would happen if there's more than 1 such page?
    sum = 0;
    std::unordered_set<int8_t> unique_constraints {};
    for (auto const &iu: incorrect_updates) {
        page_indices.fill(101);
        int32_t index = 0;
        for (auto const &p: iu) {
            page_indices[p] = index; 
            index++;
        }

        // Lambdas are difficult to force into recursion...
        std::function<void(int)> add_constraints =
        [&unique_constraints, &page_to_ordering, &add_constraints] (int p) -> void {
            if (unique_constraints.contains(p)) return; 
            if (page_indices[p] == 101) return;
            unique_constraints.insert(p);
            auto const &constraints = page_to_ordering[p];
            for (auto const &c: constraints) {
                add_constraints(c);
            }
        };

        for (auto const &p: iu) {
            unique_constraints.clear();
            if (page_indices[p] == 101) continue;
            add_constraints(p);
            if ((iu.size() - 1) / 2 == unique_constraints.size() - 1) {
                sum += p;
                break;
            }
        }
    }
    std::cout << sum << "\n";
}
