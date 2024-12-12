#include <bits/stdc++.h>
constexpr int32_t SZ = 10;
static std::array<std::string, SZ> grid {};
static std::array<std::bitset<SZ>, SZ> visited {};

bool inline in_bounds(int32_t x, int32_t y) {
    return 0 <= x && x < SZ && 0 <= y && y < SZ;
}

struct pair_hash {
    
    void hash_append (void const * key, std::size_t len, std::size_t &h) const {
        unsigned char const * p = static_cast<unsigned char const *>(key);
        unsigned char const * const e = p + len;
        for (; p < e; p++)
            h = (h ^ *p) + 1099511628211u;
    }

    std::size_t operator()(std::pair<int32_t, int32_t> const &p) const {
        std::size_t h = 14695981039346656037u;
        hash_append(&p.first, sizeof(p.first), h);
        hash_append(&p.second, sizeof(p.second), h);
        return h;
    }
};

bool valid(std::pair<int32_t, int32_t> p, char current_plant) {
    if (!in_bounds(p.first + 1, p.second) ||
        grid[p.first + 1][p.second] != current_plant) {
        return true;
    }

    if (!in_bounds(p.first - 1, p.second) ||
        grid[p.first - 1][p.second] != current_plant) {
        return true;
    }

    return false;
}

int64_t flood_fill_and_calc(int32_t i, int32_t j) {
    std::queue<std::pair<int32_t, int32_t>> q {};
    q.push({i, j});
    int64_t area {0};
    int64_t perimeter {};
    int64_t number_of_sides {};
    char plant_type = grid[i][j];
    std::vector<std::pair<int32_t, int32_t>> perim_points {};
    std::unordered_set<std::pair<int32_t, int32_t>, pair_hash> perimeter_points {};
    std::cout << "Region of plants " << plant_type << "\n";
    while (!q.empty()) {
        auto p = q.front();
        q.pop();
        if (visited[p.first].test(p.second)) continue;
        visited[p.first].set(p.second);
        area++;
        std::array<std::pair<int32_t, int32_t>, 4> new_points {{
            {p.first + 1, p.second}, {p.first - 1, p.second},
            {p.first, p.second + 1}, {p.first, p.second - 1},
        }};
        for (auto const &np: new_points) {
            if (in_bounds(np.first, np.second) &&
                grid[np.first][np.second] == plant_type) {
                q.push(np);
            } else {
                perimeter++;
                perim_points.push_back(p);
            }
        }
    }
    std::sort(perim_points.begin(), perim_points.end(),
            [] (std::pair<int32_t, int32_t> const &lhs, std::pair<int32_t, int32_t> const &rhs)
            {
                return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
            });
    auto new_end = std::unique(perim_points.begin(), perim_points.end());
    perim_points.erase(new_end, perim_points.end());
    for (auto const &p: perim_points) {
        std::cout << "{ " << p.first << ", " << p.second << " } ";
    }
    std::cout << "\n";
    for (int i = 0; i < perim_points.size(); i++) {
        int32_t index = i + 1;
        auto current_p = perim_points[i];
        if (!valid(current_p, plant_type)) continue;
        std::cout << "Trying point " << current_p.first << ", " << current_p.second << "\n";
        while (index < perim_points.size() &&
               current_p.first == perim_points[index].first &&
               current_p.second == perim_points[index].second - 1)
        {
            if (valid(perim_points[index], plant_type)) {
                current_p = perim_points[index];
                index++;
                continue;
            }
            break;
        }
        std::cout << "Found contiguous region: { "
                  << perim_points[i].first << ", " << perim_points[i].second << " } to "
                  << " { " << perim_points[index - 1].first << ", " << perim_points[index - 1].second << " } \n";
        i = index - 1;
    }

    std::cout << "Area: " << area << ", perimeter: " << perimeter << ", number of sides: " << number_of_sides << "\n";
    return area * perimeter;
}

int main() {
    std::string l;
    int index = 0;
    while (std::cin >> l) {
        grid[index] = l;
        index++;
    }

    int64_t sum {0};
    for (int i = 0; i < SZ; i++) {
        for (int j = 0; j < SZ; j++) {
            if (visited[i].test(j)) continue;
            sum += flood_fill_and_calc(i, j);
        }
    }
    std::cout << sum << "\n";
}
