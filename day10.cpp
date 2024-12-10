#include <bits/stdc++.h>
constexpr int32_t SZ = 40;
std::array<std::array<char, SZ>, SZ> grid {};
struct pair_hash {
    
    void hash_append (void const * key, std::size_t len, std::size_t &h) const {
        unsigned char const * p = static_cast<unsigned char const *>(key);
        unsigned char const * const e = p + len;
        for (; p < e; p++)
            h = (h ^ *p) + 1099511628211u;
    }

    std::size_t operator()(std::pair<int, int> const &p) const {
        std::size_t h = 14695981039346656037u;
        hash_append(&p.first, sizeof(p.first), h);
        hash_append(&p.second, sizeof(p.second), h);
        return h;
    }
};

inline bool in_bounds(int32_t x, int32_t y) {
    return 0 <= x && x < SZ && 0 <= y && y < SZ;
}

int main() {
    // P1 - BFS
    std::vector<std::pair<int, int>> starting_points {};
    std::string l;
    for (int i = 0; i < SZ; i++) {
        std::cin >> l;
        for (int j = 0; j < SZ; j++) {
            grid[i][j] = l[j];
            if (l[j] == '0') starting_points.push_back({i, j});
        }
    }
    std::unordered_set<std::pair<int, int>, pair_hash> visited_ends {};
    int32_t sum {0};
    int32_t sum_p2 {0};
    for (auto const &point: starting_points) {
        std::queue<std::pair<int, int>> q {};
        visited_ends.clear();

        q.push(point);
        while (!q.empty()) {
            auto const current_point = q.front();
            q.pop();
            if (grid[current_point.first][current_point.second] == '9') {
                sum_p2++;
                if (!visited_ends.contains(current_point)) {
                    sum++;
                    visited_ends.insert(current_point);
                }
                continue;
            }
            std::array<std::pair<int, int>, 4> surrounding_points { {
                { current_point.first + 1, current_point.second },
                { current_point.first - 1, current_point.second },
                { current_point.first, current_point.second + 1 },
                { current_point.first, current_point.second - 1 }
            } };
            for (auto const &p: surrounding_points) {
                if (in_bounds(p.first, p.second) &&
                    grid[p.first][p.second] == grid[current_point.first][current_point.second] + 1)
                {
                    q.push(p);
                }
            }
        }
    }
    std::cout << sum << "\n";
    std::cout << sum_p2 << "\n";
}
