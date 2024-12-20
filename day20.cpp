#include <bits/stdc++.h>

constexpr std::size_t SZ = 141;
bool inline in_bounds(int64_t x, int64_t y) {
    return 0 <= x && x < SZ && 0 <= y && y < SZ;
}

struct pair_hash {
    
    void hash_append (void const * key, std::size_t len, std::size_t &h) const {
        unsigned char const * p = static_cast<unsigned char const *>(key);
        unsigned char const * const e = p + len;
        for (; p < e; p++)
            h = (h ^ *p) + 1099511628211u;
    }

    std::size_t operator()(std::pair<int64_t, int64_t> const &p) const {
        std::size_t h = 14695981039346656037u;
        hash_append(&p.first, sizeof(p.first), h);
        hash_append(&p.second, sizeof(p.second), h);
        return h;
    }
};


void make_taxicab_sphere(std::vector<std::pair<int64_t, int64_t>> &sphere_points, std::pair<int64_t, int64_t> center, int64_t radius) {
    for (int64_t i = 0; i < radius; i++) {
        if (i == 0) {
            sphere_points.push_back({center.first + radius, center.second});
            sphere_points.push_back({center.first - radius, center.second});
            sphere_points.push_back({center.first, center.second + radius});
            sphere_points.push_back({center.first, center.second - radius});
            continue;
        }
        sphere_points.push_back({center.first + radius - i, center.second - i});
        sphere_points.push_back({center.first - radius + i, center.second - i});
        sphere_points.push_back({center.first + radius - i, center.second + i});
        sphere_points.push_back({center.first - radius + i, center.second + i});
    }
}

std::array<std::array<char, SZ>, SZ> grid {};
int64_t solve(int64_t x, int64_t y, int64_t max_taxicab_sphere_radius) {
    std::queue<std::pair<int64_t, int64_t>> q {};
    std::array<std::array<int64_t, SZ>, SZ> cost_map {};
    for (auto &row: cost_map) {
        row.fill(-1);
    }
    q.push({x,y});
    cost_map[x][y] = 0;
    std::pair<int64_t, int64_t> end_point;

    while (!q.empty()) {
        auto cur_point = q.front();
        q.pop();
        std::array<std::pair<int64_t, int64_t>, 4> adjacent_points {{
            {cur_point.first + 1, cur_point.second}, {cur_point.first - 1, cur_point.second},
            {cur_point.first, cur_point.second + 1}, {cur_point.first, cur_point.second - 1}
        }};
        for (auto const &p: adjacent_points) {
            if (grid[p.first][p.second] == '#') {
                continue;
            }
            if (cost_map[p.first][p.second] != -1) {
                continue;
            }
            cost_map[p.first][p.second] = cost_map[cur_point.first][cur_point.second] + 1;
            if (grid[p.first][p.second] == 'E') {
                end_point = {p};
                break;
            }
            q.push(p);
        }
    }
    
    int64_t cheats_found {0};
    std::queue<std::pair<int64_t, int64_t>> second_q {};
    second_q.push({x, y});
    while (!second_q.empty()) {
        std::unordered_set<std::pair<int64_t, int64_t>, pair_hash> unique_points {};
        auto cur_point = second_q.front();
        second_q.pop();
        for (int32_t i = 1; i <= max_taxicab_sphere_radius; i++) {
            std::vector<std::pair<int64_t, int64_t>> taxicab_sphere {};
            make_taxicab_sphere(taxicab_sphere, cur_point, i);
            for (auto const &p: taxicab_sphere) {
                if (!in_bounds(p.first, p.second)) {
                    continue;
                }
                if (cost_map[p.first][p.second] == -1) {
                    continue;
                }
                if (cost_map[p.first][p.second] == cost_map[cur_point.first][cur_point.second] + 1) {
                    second_q.push(p);
                    continue;
                }
                if (unique_points.contains(p)) continue;
                if (cost_map[p.first][p.second] - i > cost_map[cur_point.first][cur_point.second]) {
                    unique_points.insert(p);
                    if (cost_map[p.first][p.second] - i - cost_map[cur_point.first][cur_point.second] >= 100) {
                        cheats_found++;
                    }
                }
            }
        }
    }
    std::cout << cheats_found << "\n";
    return cost_map[end_point.first][end_point.second];
}

int main() {
    std::string l;
    std::pair<int64_t, int64_t> start;
    for (int i = 0; i < SZ; i++) {
        std::cin >> l;
        for (int j = 0; j < SZ; j++) {
            if (l[j] == 'S') start = {i, j};
            grid[i][j] = l[j];
        }
    }
    solve(start.first, start.second, 2);
    solve(start.first, start.second, 20);
}
