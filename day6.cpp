// Requires C++20 for .contains
#include <bits/stdc++.h>
constexpr int SZ {130};
constexpr int8_t fwd {1};
constexpr int8_t bwd {0};
static std::array<std::array<char, SZ>, SZ> grid;
static std::array<std::array<char, SZ>, SZ> grid2;

char get_new_direction(char dir) {
    switch (dir) {
        case '^':
            return '>';
        case '>':
            return 'v';
        case 'v':
            return '<';
        case '<':
            return '^';
        default:
            return dir;
    }
}

bool OK(int x, int y) {
    if (x > SZ - 1) return false;
    if (x < 0) return false;
    if (y > SZ - 1) return false;
    if (y < 0) return false;
    return true;
}

// Hash is fnva1 function, stolen from presentation by Victor Ciura
// "So You Think You Can Hash", very interesting
struct hash_for_obstacles {
    
    void append_hash(void const * key, std::size_t len, std::size_t &h) const {
        unsigned char const * p = static_cast<unsigned char const *>(key);
        unsigned char const * const e = p + len;
        for (; p < e; p++)
            h = (h ^ *p) + 1099511628211u;
    }

    std::size_t operator()(std::tuple<int, int, char> const &to_hash) const {
        std::size_t h = 14695981039346656037u;
        append_hash(&std::get<0>(to_hash), sizeof(std::get<0>(to_hash)), h);
        append_hash(&std::get<1>(to_hash), sizeof(std::get<1>(to_hash)), h);
        append_hash(&std::get<2>(to_hash), sizeof(std::get<2>(to_hash)), h);
        return h;
    }
};

std::pair<int, int> move_guard(std::pair<int, int> g, int8_t fwd_bwd, char dir) {
    std::pair<int, int> new_g;
    switch (dir) {
        case '>':
            new_g.first = g.first;
            new_g.second = fwd_bwd ? g.second + 1 : g.second - 1;
            return new_g;
        case '^':
            new_g.first = fwd_bwd ? g.first - 1 : g.first + 1;
            new_g.second = g.second;
            return new_g;
        case '<':
            new_g.first = g.first;
            new_g.second = fwd_bwd ? g.second - 1 : g.second + 1;
            return new_g;
        case 'v':
            new_g.first = fwd_bwd ? g.first + 1 : g.first - 1;
            new_g.second = g.second;
            return new_g;
        default:
            return g;
    }
}

bool part2(std::array<std::array<char, SZ>, SZ> &grid_copy, int x, int y, std::pair<int, int> guard) {
    std::unordered_set<std::tuple<int, int, char>, hash_for_obstacles> obstacles_already_hit {};
    grid_copy[x][y] = '#';
    char current_dir = grid_copy[guard.first][guard.second];
    bool loop_found = false;
    while (OK(guard.first, guard.second)) {
        if (grid_copy[guard.first][guard.second] == '#') {
            if (obstacles_already_hit.contains({guard.first, guard.second, current_dir})) {
                loop_found = true;
                break;
            }
            obstacles_already_hit.insert({guard.first, guard.second, current_dir});
            guard = move_guard(guard, bwd, current_dir);
            current_dir = get_new_direction(current_dir);
        }
        guard = move_guard(guard, fwd, current_dir);
    }
    grid_copy[x][y] = '.';
    if (loop_found) {
        return true;
    }
    return false;
}

int main() {
    std::pair<int, int> guard {};
    std::pair<int, int> original_guard {};
    std::string l;
    for (int i = 0; i < SZ; i++) {
        std::cin >> l;
        for (int j = 0; j < SZ; j++) {
            if (l[j] == '^') guard = {i, j};
            grid[i][j] = l[j]; 
            grid2[i][j] = l[j];
        }
    }
    original_guard = guard;

    char current_dir = grid[guard.first][guard.second];
    int location_count {0};
    while (OK(guard.first, guard.second)) {
        if (grid[guard.first][guard.second] == '#') {
            guard = move_guard(guard, bwd, current_dir);
            current_dir = get_new_direction(current_dir);
        }
        if (grid[guard.first][guard.second] != 'X') location_count++;
        grid[guard.first][guard.second] = 'X';
        guard = move_guard(guard, fwd, current_dir);
    }
    std::cout << location_count << "\n";
    
    // Brute force, takes a while but nothing a modern computer can't handle
    // It could be reduced by taking a look at the output of p1 and shaving off
    // the rows where the guard never goes. In my case it was almost 2/3 of the grid
    // Time taken by brute force:
    // real    0m1.295s
    // user    0m1.293s
    // sys     0m0.000s
    int loops_possible {0};
    for (int m = 0; m < SZ; m++) {
        for (int n = 0; n < SZ; n++) {
            if (grid2[m][n] == '#') {
                continue; // Already existing obstacle, nothing to check
            } else if (grid2[m][n] != '^') { // Not guard, check if a new obstacle loops
                loops_possible +=
                    part2(grid2, m, n, original_guard) ?
                    1 : 0;
            }
        }
    }
    std::cout << loops_possible << "\n";
}
