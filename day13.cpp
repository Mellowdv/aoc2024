#include <bits/stdc++.h>
int64_t solve(int64_t button_a_x, int64_t button_a_y, int64_t button_b_x, int64_t button_b_y, int64_t target_x, int64_t target_y) {
    int64_t best_coins = std::numeric_limits<int64_t>::max();

    std::cout << target_x << " " << target_y << "\n";
    int64_t current_x, current_y;
    int a_presses = 0;
    int b_presses = 0;
    for (; a_presses <= 100; a_presses++) {
        b_presses = 0;
        current_x = a_presses * button_a_x;
        current_y = a_presses * button_a_y;
        if (current_x == target_x && current_y == target_y) {
            best_coins = std::min(best_coins, static_cast<int64_t>(a_presses * 3 + b_presses));
        }
        if (a_presses == 80) {
            std::cout << current_x << " " << current_y << "\n";
        }
        for (; b_presses <= 100; b_presses++) {
            if (current_x + b_presses * button_b_x == target_x &&
                current_y + b_presses * button_b_y == target_y) {
                best_coins = std::min(best_coins, static_cast<int64_t>(a_presses * 3 + b_presses));
                break;
            }
        }
    }
    return (best_coins == std::numeric_limits<int64_t>::max() ? 0 : best_coins);
}

int64_t solve_p2(int64_t button_a_x, int64_t button_a_y, int64_t button_b_x, int64_t button_b_y, int64_t target_x, int64_t target_y) {
    target_x += 10000000000000;
    target_y += 10000000000000;
    // Binary search version maybe?
}

int main() {
    std::string l;
    int64_t button_a_x, button_a_y, button_b_x, button_b_y, target_x, target_y;
    int64_t best_coins = std::numeric_limits<int64_t>::max();
    int64_t sum {0};
    while (std::getline(std::cin, l)) {
        auto new_it = l.find_first_of("+");
        button_a_x = std::atoll(l.data() + new_it);
        new_it = l.find_first_of("+", new_it + 1);
        button_a_y = std::atoll(l.data() + new_it);

        std::getline(std::cin, l);
        new_it = l.find_first_of("+");
        button_b_x = std::atoll(l.data() + new_it);
        new_it = l.find_first_of("+", new_it + 1);
        button_b_y = std::atoll(l.data() + new_it);

        std::getline(std::cin, l);
        new_it = l.find_first_of("=");
        target_x = std::atoll(l.data() + new_it + 1);
        new_it = l.find_first_of("=", new_it + 1);
        target_y = std::atoll(l.data() + new_it + 1);
        std::getline(std::cin, l);
        sum += solve(button_a_x, button_a_y, button_b_x, button_b_y, target_x, target_y);
    }
    std::cout << sum << "\n";
}
