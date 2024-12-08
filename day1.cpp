#include <bits/algorithmfwd.h>
#include <bits/stl_algobase.h>
#include <bits/stl_algo.h>
#include <bits/cpp_type_traits.h>
#include <bits/ranges_algo.h>
#include <bits/stdc++.h>
#include <ranges>

static std::array<int, 1000> left_side {};
static std::array<int, 1000> right_side {};
std::unordered_map<int, int> frequency_counter {};
auto partial_fold_left(decltype(std::views::zip(left_side, right_side)) &foldable) {
    auto plus = [] (int sum, std::tuple<int, int> zipped)
        { return sum + std::abs(std::get<0>(zipped) - std::get<1>(zipped)); };
    return std::ranges::fold_left(foldable, 0, plus); 
}
auto operator|(decltype(std::views::zip(left_side, right_side)) &&foldable, decltype(partial_fold_left) p_f_l) {
    return p_f_l(foldable);
}

int main() {
    for (int i = 0; i < 1000; i++) {
        std::cin >> left_side[i] >> right_side[i];
        frequency_counter[right_side[i]]++;
    }
    std::sort(left_side.begin(), left_side.end());
    std::sort(right_side.begin(), right_side.end());

    // Just an attempt at making this look neat so we don't have to do what's below...
    auto attempt = std::views::zip(left_side, right_side) | partial_fold_left;
    std::cout << attempt << "\n";

    auto zipped = std::views::zip(left_side, right_side);
    auto d1_result = std::ranges::fold_left(zipped, 0,
                        [] (int sum, std::tuple<int, int> zipped)
                        { return sum + std::abs(std::get<0>(zipped) - std::get<1>(zipped)); });

    auto d2_result = std::ranges::fold_left(left_side, 0,
                        [] (int sum, int lhs_element)
                        { return sum +
                            (frequency_counter.contains(lhs_element) ?
                                lhs_element * frequency_counter[lhs_element] :
                                0
                            );
                        });
    std::cout << d1_result << "\n";
    std::cout << d2_result << "\n";
}
