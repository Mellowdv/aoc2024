#include <bits/stdc++.h>
constexpr int32_t SZ = 140;
static std::array<std::string, SZ> grid {};
static std::array<std::bitset<SZ>, SZ> visited {};

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

bool inline in_bounds(int32_t x, int32_t y) {
    return 0 <= x && x < SZ && 0 <= y && y < SZ;
}

void print_analyzed_polygon_debug(std::unordered_set<std::pair<int32_t, int32_t>, pair_hash> blocks) {
    std::array<std::array<char, SZ>, SZ> g {};
    for (auto &row: g) {
      row.fill('.');
    }
    for (auto v: blocks) {
      g[v.first][v.second] = 'X';
    }
    for (auto &row: g) {
      for (auto &ch: row) {
        std::cout << ch;
      }
      std::cout << "\n";
    }
}


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

bool contiguous(std::pair<int32_t, int32_t> const &lhs, std::pair<int32_t, int32_t> const &rhs) {
    return (lhs.first == rhs.first) && (lhs.second == rhs.second - 1);
}

bool double_edge(std::pair<int32_t, int32_t> p, char current_plant, std::unordered_set<std::pair<int32_t, int32_t>, pair_hash> const &polygon_blocks) {
    if (in_bounds(p.first + 1, p.second) &&
        in_bounds(p.first - 1, p.second) &&
        grid[p.first + 1][p.second] == current_plant &&
        grid[p.first - 1][p.second] == current_plant) {
      if (polygon_blocks.contains({p.first + 1, p.second}) &&
          polygon_blocks.contains({p.first - 1, p.second})) {
          return true;
      }
    }
    return false;
}

bool is_same_edge(std::pair<int32_t, int32_t> p, std::pair<int32_t, int32_t> p2, char plant_type, std::unordered_set<std::pair<int32_t, int32_t>, pair_hash> const &polygon_blocks) {
  if (in_bounds(p.first + 1, p.second) && in_bounds(p2.first + 1, p2.second) && grid[p.first + 1][p.second] == plant_type
      && grid[p2.first + 1][p2.second] == plant_type) {
      if (polygon_blocks.contains({p.first + 1, p.second}) &&
          polygon_blocks.contains({p2.first + 1, p.second}))
        return true;

  }
  if (in_bounds(p.first - 1, p.second) && in_bounds(p2.first - 1, p2.second) && grid[p.first - 1][p.second] == plant_type
      && grid[p2.first - 1][p2.second] == plant_type) {
      if (polygon_blocks.contains({p.first - 1, p.second}) &&
          polygon_blocks.contains({p2.first - 1, p.second}))
        return true;

  }
  if (!in_bounds(p.first + 1, p.second) && !in_bounds(p2.first + 1, p.second)) return true;
  if (!in_bounds(p.first - 1, p.second) && !in_bounds(p2.first - 1, p.second)) return true;
  return false;
}

bool is_edge(std::pair<int32_t, int32_t> p, char current_plant, std::unordered_set<std::pair<int32_t, int32_t>, pair_hash> const &polygon_blocks) {
    if (in_bounds(p.first + 1, p.second) && grid[p.first + 1][p.second] == current_plant) {
      if (polygon_blocks.contains({p.first + 1, p.second})) return true;
    }
    if (in_bounds(p.first - 1, p.second) && grid[p.first - 1][p.second] == current_plant) {
      if (polygon_blocks.contains({p.first - 1, p.second})) return true;
    }
    return false;
}

std::pair<int64_t, int64_t> flood_fill_and_calc(int32_t i, int32_t j) {
    std::queue<std::pair<int32_t, int32_t>> q {};
    q.push({i, j});
    int64_t area {0};
    int64_t perimeter {};
    char plant_type = grid[i][j];
    std::vector<std::pair<int32_t, int32_t>> perim_points {};
    std::unordered_set<std::pair<int32_t, int32_t>, pair_hash> polygon_blocks {};
    std::cout << "Region of plants " << plant_type << "\n";
    while (!q.empty()) {
        auto p = q.front();
        q.pop();
        if (visited[p.first].test(p.second)) continue;
        polygon_blocks.insert(p);
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
                perim_points.push_back(np);
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
    print_analyzed_polygon_debug(polygon_blocks);
    for (auto const &p: perim_points) {
        std::cout << "{ " << p.first << ", " << p.second << " } ";
    }
    std::cout << "\n";
    int32_t contiguous_regions = 0;
    for (int i = 0; i < perim_points.size(); i++) {
        int32_t index = i + 1;
        auto current_point = perim_points[i];
        bool is_double_edge = double_edge(current_point, plant_type, polygon_blocks);
        bool found_region = is_edge(current_point, plant_type, polygon_blocks);
        std::cout << "Analyzed point is " << current_point.first << ", " << current_point.second << "\n";
        while (index < perim_points.size() && contiguous(current_point, perim_points[index])) {
            std::cout << "Result of contig: " << contiguous(current_point, perim_points[index]) << "\n";
            if (!is_edge(perim_points[index], plant_type, polygon_blocks) ||
                !is_same_edge(current_point, perim_points[index], plant_type, polygon_blocks)) {
                std::cout << "Aaaaand I done guffed\n";
                found_region = false || found_region;
                break;
            }
            found_region = true;
            is_double_edge = is_double_edge || double_edge(perim_points[index], plant_type, polygon_blocks);
            current_point = perim_points[index];
            index++;
        }
        index--;
        if (found_region) {
            contiguous_regions++;
          std::cout << "Found region is from " << perim_points[i].first << ", " << perim_points[i].second << " to "
                    << perim_points[index].first << ", " << perim_points[index].second << "\n";
        }
        if (is_double_edge) contiguous_regions++;
        i = index;
    }

    std::cout << "Area: " << area << ", perimeter: " << perimeter << ", number of sides: " << contiguous_regions * 2 << "\n";
    return {area * perimeter, area * contiguous_regions * 2};
}

int main() {
    std::string l;
    int index = 0;
    while (std::cin >> l) {
        grid[index] = l;
        index++;
    }

    int64_t sum {0};
    int64_t sum_2 {0};
    for (int i = 0; i < SZ; i++) {
        for (int j = 0; j < SZ; j++) {
            if (visited[i].test(j)) continue;
            auto result = flood_fill_and_calc(i, j);
            sum += result.first;
            sum_2 += result.second;
        }
    }
    std::cout << sum << "\n";
    std::cout << sum_2 << "\n";
}
