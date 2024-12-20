#include <bits/stdc++.h>
std::unordered_set<std::string> available_towels {};

std::unordered_map<std::string, int64_t> cache {};
int64_t count_number_of_ways(std::string_view const& pattern) {
    int64_t count {0};
    if (cache.contains(std::string(pattern))) {
        return cache[std::string(pattern)];
    }
    if (pattern == "") {
        return 1;
    }
    for (auto const &towel: available_towels) {
        if (pattern.substr(0, towel.size()) == towel) {
            auto suffix = pattern.substr(towel.size());
            count += count_number_of_ways(suffix);
        }
    }

    cache[std::string(pattern)] = count;
    return count;
}

using std::operator""sv;
int main() {
    std::string l;
    std::getline(std::cin, l);
    for (auto towel: std::views::split(l, ", "sv)) {
        available_towels.insert(to<std::string>(towel));
    }

    std::cin.ignore();
    int64_t possible_count {};
    int64_t combination_count {};
    while (std::cin >> l) {
        int64_t count = count_number_of_ways(l);
        if (count > 0) {
            std::cout << "Matched " << l << "\n";
            possible_count++;
        } else {
            std::cout << l << " unmatched!\n";
        }
        combination_count += count;
    }
    std::cout << possible_count << "\n";
    std::cout << combination_count << "\n";
}
