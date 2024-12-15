#include <bits/stdc++.h>
#include <strings.h>
constexpr std::size_t WIDTH = 101;
constexpr std::size_t HEIGHT = 103;
static std::array<std::array<char, WIDTH>, HEIGHT> grid {};

template<typename V>
V mod(const V& a, const V& b)
{
    return (a % b + b) % b;
}

std::array<std::pair<int64_t, int64_t>, 500> robots {};
std::array<std::pair<int64_t, int64_t>, 500> velocities {};

int main() {
    for (auto &row: grid) {
        row.fill('.');
    }

    std::string line;
    int64_t pos_x, pos_y, d_x, d_y;
    int64_t middle_row = (HEIGHT - 1) / 2;
    int64_t middle_column = (WIDTH - 1) / 2;
    int64_t quadrant1, quadrant2, quadrant3, quadrant4;
    quadrant1 = quadrant2 = quadrant3 = quadrant4 = 0;
    int64_t index = 0;
    while (std::getline(std::cin, line)) {
        auto new_it = line.find_first_of('=') + 1;
        pos_x = std::atoll(line.data() + new_it);

        new_it = line.find_first_of(',', new_it) + 1;
        pos_y = std::atoll(line.data() + new_it);

        new_it = line.find_first_of('=', new_it) + 1;
        d_x = std::atoll(line.data() + new_it);

        new_it = line.find_first_of(',', new_it) + 1;
        d_y = std::atoll(line.data() + new_it);
        robots[index] = {pos_x, pos_y};
        velocities[index] = {d_x, d_y};
        index++;

        pos_x += 100 * d_x;
        pos_y += 100 * d_y;
        pos_x = mod(pos_x, (int64_t)WIDTH);
        pos_y = mod(pos_y, (int64_t)HEIGHT);
        std::cout << pos_x << " " << pos_y << "\n";
        grid[pos_y][pos_x] = grid[pos_y][pos_x] == '.' ? '1' : grid[pos_y][pos_x] + 1;
        if (pos_y == middle_row || pos_x == middle_column) continue;
        if (pos_x < middle_column) {
            if (pos_y < middle_row) {
                quadrant1++;
            } else {
                quadrant4++;
            }
        } else {
            if (pos_y < middle_row) {
                quadrant2++;
            } else {
                quadrant3++;
            }
        }
    }
    for (auto const &row: grid) {
        for (auto const &ch: row) {
            std::cout << ch;
        }
        std::cout << "\n";
    }
    std::cout << quadrant1 * quadrant2 * quadrant3 * quadrant4 << "\n";

    char ch;
    int64_t seconds = 0;
    std::cin.ignore();
    int attempts = 50000;
    while (--attempts) {
        std::cout << "\n";
        for (auto &row: grid) {
            row.fill('.');
        }
        std::cout << "PICTURE NUMBER " << seconds << "\n";
        for (int i = 0; i < robots.size(); i++) {
            pos_x = robots[i].first;
            pos_y = robots[i].second;
            pos_x += seconds * velocities[i].first;
            pos_y += seconds * velocities[i].second;
            pos_x = mod(pos_x, (int64_t)WIDTH);
            pos_y = mod(pos_y, (int64_t)HEIGHT);
            grid[pos_y][pos_x] = grid[pos_y][pos_x] == '.' ? '1' : grid[pos_y][pos_x] + 1;
        }
        for (auto const &row: grid) {
            for (auto const &ch: row) {
                std::cout << ch;
            }
            std::cout << "\n";
        }
        seconds++;
    }
    return 0;
}
