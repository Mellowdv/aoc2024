#include <bits/stdc++.h>
#include <string>

using ii = std::pair<int, int>;
constexpr std::pair<int, int> NOPE_NUM {0, 0};
constexpr std::array<std::pair<int, int>, 12> numerical_keypad {{
    {0, 1}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1},
    {2, 2}, {3, 0}, {3, 1}, {3, 2}, {0, 2}, NOPE_NUM
}};

constexpr ii NOPE_DIR {1, 0};
// Legend: < == 0, v == 1, > == 2, ^ == 3, A == 4
constexpr std::array<ii, 6> directional_keypad {{
    {0, 0}, {0, 1}, {0, 2}, {1, 1}, {1, 2}, NOPE_DIR
}};
int get_directional_index(char ch) {
    switch (ch) {
        case '<':
            return 0;
        case 'v':
            return 1;
        case '>':
            return 2;
        case '^':
            return 3;
        case 'A':
            return 4;
        default:
            return 5;
    }
}

// TO DO: deal with the fact that >>^ is better than >^> for example
// That is: if a direction repeats, we need to try and pack it up together to minimize button presses
std::string get_directions_for_numerical(std::string const& code) {
    std::string keypad_input = "";
    ii start = numerical_keypad[10];
    for (auto const &ch: code) {
        auto keypad_pos = numerical_keypad[(ch == 'A' ? 10 : ch - '0')];
        auto dx = keypad_pos.first - start.first;
        auto dy = keypad_pos.second - start.second;
        while (dx != 0 || dy != 0) {
            if (dx < 0 && !(start.first - 1 == NOPE_NUM.first && start.second == NOPE_NUM.second)) {
                start.first--;
                dx++;
                keypad_input += 'v';
                continue;
            }
            if (dy < 0 && !(start.first == NOPE_NUM.first && start.second - 1 == NOPE_NUM.second)) {
                start.second--;
                dy++;
                keypad_input += '<';
                continue;
            }

            if (dx > 0 && !(start.first + 1 == NOPE_NUM.first && start.second == NOPE_NUM.second)) {
                start.first++;
                dx--;
                keypad_input += '^';
                continue;
            }
            if (dy > 0 && !(start.first == NOPE_NUM.first && start.second + 1 == NOPE_NUM.second)) {
                start.second++;
                dy--;
                keypad_input += '>';
                continue;
            }
        }
        keypad_input += 'A';
    }
    return keypad_input;
}

// v<<A>^>A<A>A<AAv>A^Av<AAA^>A
std::string get_directions_for_directional(std::string const &sequence) {
    std::cout << "Expanding sequence " << sequence << "\n";
    std::string keypad_input = "";
    ii start = directional_keypad[4];
    for (auto const &ch: sequence) {
        std::cout << "Sequence element " << ch << ", current pos: " << start.first << ", " << start.second << "\n";
        auto keypad_pos = directional_keypad[get_directional_index(ch)];
        auto dx = keypad_pos.first - start.first;
        auto dy = keypad_pos.second - start.second;
        while (dx != 0 || dy != 0) {
            if (dx < 0 && !(start.first - 1 == NOPE_DIR.first && start.second == NOPE_DIR.second)) {
                start.first--;
                dx++;
                keypad_input += 'v';
                continue;
            }
            if (dy < 0 && !(start.first == NOPE_DIR.first && start.second - 1 == NOPE_DIR.second)) {
                start.second--;
                dy++;
                keypad_input += '<';
                continue;
            }

            if (dx > 0 && !(start.first + 1 == NOPE_DIR.first && start.second == NOPE_DIR.second)) {
                start.first++;
                dx--;
                keypad_input += '^';
                continue;
            }
            if (dy > 0 && !(start.first == NOPE_DIR.first && start.second + 1 == NOPE_DIR.second)) {
                start.second++;
                dy--;
                keypad_input += '>';
                continue;
            }
        }
        keypad_input += 'A';
    }
    return keypad_input;
}

int main() {
    std::string step1 = get_directions_for_numerical("379A");

    std::string step2 = get_directions_for_directional(step1);
    std::string step3 = get_directions_for_directional(step2);
    std::cout << step3 << "\n";
    std::cout << step3.size() << "\n";
}
