#include <bits/stdc++.h>
constexpr std::size_t SZ = 50;
constexpr char ROBOT = '@';
constexpr char BOX = 'O';
constexpr char WALL = '#';
constexpr char FLOOR = '.';

static std::array<std::array<char, SZ>, SZ> grid {};
static std::array<std::array<char, SZ * 2>, SZ> grid_p2 {};
// Stores information about robot's position and length including all pushed boxes
struct Robot {
    int64_t h;
    int64_t w;
};

std::pair<int, int> delta(char dir) {
    switch (dir)
    {
        case '^':
            return {-1, 0};
        case 'v':
            return {1, 0};
        case '>':
            return {0, 1};
        case '<':
            return {0, -1};
        default:
            std::cout << "Bad move!\n";
            return {0, 0};
    }
}

void move(Robot &r, char const &dir) {
    auto const d {delta(dir)};

    auto &new_pos = grid[r.h + d.first][r.w + d.second];
    if (new_pos == FLOOR) {
        new_pos = ROBOT;
        grid[r.h][r.w] = FLOOR;
        r.h += d.first;
        r.w += d.second;
        return;
    } else if (new_pos == BOX) {
        int64_t h, w;
        h = r.h + d.first;
        w = r.w + d.second;
        int64_t d_count {1};
        while (grid[h][w] == BOX) { // Find all the boxes in front of the robot
            h += d.first;
            w += d.second;
            d_count++;
        }

        if (grid[h][w] == WALL) { // Can't move, wall
            return;
        }

        while (h != r.h || w != r.w) { // Swap all the boxes and robot into new pos
            std::swap(grid[h][w], grid[h - d.first][w - d.second]);
            h -= d.first;
            w -= d.second;
        }
        r.h = h + d.first;
        r.w = w + d.second;

        return;
    } // In case of wall, do nothing
}

bool inline free_space_in_front(int64_t row, int64_t col1, int64_t col2) {
    return (grid_p2[row][col1] == FLOOR && grid_p2[row][col2] == FLOOR);
}

bool inline wall_in_front(int64_t row, int64_t col1, int64_t col2) {
    return (grid_p2[row][col1] == WALL || grid_p2[row][col2] == WALL);
}

bool inline another_box(int64_t row, int64_t col1, int64_t col2) {
    return (grid_p2[row][col1] == '[' && grid_p2[row][col2] == ']');
}

bool inline left_box(int64_t row, int64_t col1, int64_t col2) {
    return (grid_p2[row][col1] == ']');
}

bool inline right_box(int64_t row, int64_t col1, int64_t col2) {
    return (grid_p2[row][col2] == '[');
}

bool inline double_box(int64_t row, int64_t col1, int64_t col2) {
    return (grid_p2[row][col1] == ']' && grid_p2[row][col2] == '[');
}

bool can_box_move(int64_t box_row, std::pair<int64_t, int64_t> box_cols, std::pair<int64_t, int64_t> d) {
    int64_t new_row = box_row + d.first;
    if (wall_in_front(new_row, box_cols.first, box_cols.second)) {
        return false;
    }

    if (free_space_in_front(new_row, box_cols.first, box_cols.second)) {
        return true;
    }

    if (another_box(new_row, box_cols.first, box_cols.second)) {
        return can_box_move(new_row, box_cols, d);
    }

    if (double_box(new_row, box_cols.first, box_cols.second)) {
        return (can_box_move(new_row, {box_cols.first - 1, box_cols.first}, d) &&
               can_box_move(new_row, {box_cols.second, box_cols.second + 1}, d));
    }

    if (left_box(new_row, box_cols.first, box_cols.second)) {
        return (can_box_move(new_row, {box_cols.first - 1, box_cols.first}, d));
    }

    if (right_box(new_row, box_cols.first, box_cols.second)) {
        return (can_box_move(new_row, {box_cols.second, box_cols.second + 1}, d));
    }
    return false;
}

void move_box(int64_t box_row, std::pair<int64_t, int64_t> box_cols, std::pair<int64_t, int64_t> d) {
    auto const new_row = box_row + d.first;
    if (free_space_in_front(new_row, box_cols.first, box_cols.second)) {
        grid_p2[box_row + d.first][box_cols.first] = grid_p2[box_row][box_cols.first];
        grid_p2[box_row + d.first][box_cols.second] = grid_p2[box_row][box_cols.second];
        grid_p2[box_row][box_cols.first] = FLOOR;
        grid_p2[box_row][box_cols.second] = FLOOR;
        return;
    } else {
        if (another_box(new_row, box_cols.first, box_cols.second)) {
            move_box(new_row, box_cols, d);
        } else if (double_box(new_row, box_cols.first, box_cols.second)) {
            move_box(new_row, {box_cols.first - 1, box_cols.first}, d);
            move_box(new_row, {box_cols.second, box_cols.second + 1}, d);
        } else if (left_box(new_row, box_cols.first, box_cols.second)) {
            move_box(new_row, {box_cols.first - 1, box_cols.first}, d);
        } else if (right_box(new_row, box_cols.first, box_cols.second)) {
            move_box(new_row, {box_cols.second, box_cols.second + 1}, d);
        }
        grid_p2[box_row + d.first][box_cols.first] = grid_p2[box_row][box_cols.first];
        grid_p2[box_row + d.first][box_cols.second] = grid_p2[box_row][box_cols.second];
        grid_p2[box_row][box_cols.first] = FLOOR;
        grid_p2[box_row][box_cols.second] = FLOOR;
        return;
    }
}

void move2(Robot &r, char const &dir) {
    auto const d = delta(dir);
    auto &new_pos = grid_p2[r.h + d.first][r.w + d.second];
    if (new_pos == FLOOR) {
        new_pos = ROBOT;
        grid_p2[r.h][r.w] = FLOOR;
        r.h += d.first;
        r.w += d.second;
        return;
    } else if (new_pos == '[' || new_pos == ']') {
        auto box_cols = (new_pos == '[' ? std::make_pair(r.w, r.w + 1) : std::make_pair(r.w - 1, r.w));
        if ((dir == '^' || dir == 'v')) {
            if (can_box_move(r.h + d.first, box_cols, d)) {
                move_box(r.h + d.first, box_cols, d);
                grid_p2[r.h][r.w] = '.';
                r.h += d.first;
                r.w += d.second;
                grid_p2[r.h][r.w] = '@';
            }
            return;
        } else {
            int64_t h, w;
            h = r.h + d.first;
            w = r.w + d.second;
            int64_t d_count {1};
            while (grid_p2[h][w] == '[' || grid_p2[h][w] == ']') { // Find all the boxes in front of the robot
                h += d.first;
                w += d.second;
                d_count++;
            }

            if (grid_p2[h][w] == WALL) { // Can't move, wall
                return;
            }

            while (h != r.h + d.first || w != r.w + d.first) { // Swap all the boxes and robot into new pos
                std::swap(grid_p2[h][w], grid_p2[h - d.first][w - d.second]);
                h -= d.first;
                w -= d.second;
            }
            grid_p2[r.h][r.w] = '.';
            r.h = h + d.first;
            r.w = w + d.second;

            return;
        }
    } // In case of wall, do nothing
}

int main() {
    std::string l;
    Robot r;
    Robot r2;
    for (int i = 0; i < SZ; i++) {
        std::cin >> l;
        int k = 0;
        for (int j = 0; j < SZ; j++, k += 2) {
            grid[i][j] = l[j];
            if (l[j] == ROBOT) {
                r = Robot {i, j};
                r2 = Robot {i, k};
                grid_p2[i][k] = '@';
                grid_p2[i][k + 1] = '.';
            } else if (l[j] == '#') {
               grid_p2[i][k] = '#';
               grid_p2[i][k + 1] = '#';
            } else if (l[j] == 'O') {
               grid_p2[i][k] = '[';
               grid_p2[i][k + 1] = ']';
            } else {
               grid_p2[i][k] = '.';
               grid_p2[i][k + 1] = '.';
            }
        }
    }
    std::cin.ignore();

    while (std::getline(std::cin, l)) {
        for (auto const &ch: l) {
            move(r, ch);
            move2(r2, ch);
        }
    }

    int64_t sum {0};
    for (int i = 0; i < SZ; i++) {
        for (int j = 0; j < SZ; j++) {
            if(grid[i][j] == BOX) {
                sum += 100 * i + j;
            }
        }
    }
    std::cout << sum << "\n";

    int64_t sum2 {0};
    for (int i = 0; i < SZ; i++) {
        for (int j = 0; j < SZ * 2; j++) {
            if (grid_p2[i][j] == '[') {
                sum2 += 100 * i + j;
            }
        }
    }
    std::cout << sum2 << "\n";
    return 0;
}
