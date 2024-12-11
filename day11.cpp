#include <bits/stdc++.h>
std::unordered_map<std::string, std::array<int64_t, 76>> cache {};
int64_t cache_hits {};

int64_t expand_and_cache_recursive(
        std::string elem,
        int64_t blinks_elapsed,
        int64_t blinks_max) {
    if (blinks_elapsed == blinks_max) return 1;

    if (!cache.contains(elem)) {
        cache.insert({elem, {}});
        cache[elem].fill(0);
    } else {
        if (cache[elem][blinks_max - blinks_elapsed] != 0) {
            cache_hits++;
            return cache[elem][blinks_max - blinks_elapsed];
        }
    }

    int64_t current_result;
    if (elem == "0") {
        current_result = expand_and_cache_recursive("1", blinks_elapsed + 1, blinks_max);
    } else if (elem.size() % 2 == 0) {
        auto lhs = std::string(elem.begin(), elem.begin() + (elem.size() / 2));
        auto tmp_it = elem.begin() + (elem.size() / 2);
        while (*tmp_it == '0' && tmp_it < elem.end() - 1) {
            ++tmp_it;
        }
        auto rhs = std::string(tmp_it, elem.end());
        current_result = expand_and_cache_recursive(lhs, blinks_elapsed + 1, blinks_max)
                 + expand_and_cache_recursive(rhs, blinks_elapsed + 1, blinks_max);
    } else {
        int64_t num = std::stoll(elem);
        num *= 2024;
        std::string new_elem = std::to_string(num);
        current_result = expand_and_cache_recursive(new_elem, blinks_elapsed + 1, blinks_max);
    }

    cache[elem][blinks_max - blinks_elapsed] = current_result;
    return current_result;
}

int main() {
    std::string s;
    int64_t sum = 0;
    int64_t alt_sum = 0;
    while (std::cin >> s) {
        sum += expand_and_cache_recursive(s, 0, 25);
        alt_sum += expand_and_cache_recursive(s, 0, 75);
    }

    std::cout << "There were " << cache_hits << " cache hits\n";
    std::cout << sum << "\n";
    std::cout << alt_sum << "\n";
}
