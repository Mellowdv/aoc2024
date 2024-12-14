#include <bits/stdc++.h>

int64_t solve(int64_t button_a_x, int64_t button_a_y, int64_t button_b_x,
              int64_t button_b_y, int64_t target_x, int64_t target_y) {
  int64_t m = (target_y * button_a_x - target_x * button_a_y) /
              (button_a_x * button_b_y - button_a_y * button_b_x);
  int64_t n = (target_x - (button_b_x * m)) / button_a_x;

  if (button_a_x * n + button_b_x * m == target_x &&
      button_a_y * n + button_b_y * m == target_y) {
    return 3 * n + m;
  } else {
    return 0;
  }
}

int main() {
  std::string l;
  int64_t button_a_x, button_a_y, button_b_x, button_b_y, target_x, target_y;
  int64_t best_coins = std::numeric_limits<int64_t>::max();
  int64_t sum{0};
  int64_t sum_p2{0};
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
    sum += solve(button_a_x, button_a_y, button_b_x, button_b_y, target_x,
                 target_y);
    sum_p2 += solve(button_a_x, button_a_y, button_b_x, button_b_y,
                    target_x + 10000000000000, target_y + 10000000000000);
  }

  std::cout << sum << "\n";
  std::cout << sum_p2 << "\n";
}
