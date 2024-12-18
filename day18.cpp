#include <bits/stdc++.h>
#include <ranges>
constexpr std::size_t SZ = 71;
constexpr int32_t BYTES = 1024;
static std::array<std::array<char, SZ>, SZ> grid {};

bool inline in_bounds(std::pair<int32_t, int32_t> p) {
    return (0 <= p.first && p.first < SZ && 0 <= p.second && p.second < SZ);
}

struct Bfs_Node {
    std::pair<int32_t, int32_t> p;
    int32_t cost;
};

int32_t bfs() {
    std::queue<Bfs_Node> q {};
    std::array<std::bitset<SZ>, SZ> visited_map {};
    Bfs_Node start {{0,0}, 0};
    q.push(start);
    visited_map[0].set(0);

    int32_t best_cost {std::numeric_limits<int32_t>::max()};
    while (!q.empty()) {
        auto const cur = q.front();
        q.pop();
        if (cur.p.first == SZ - 1 && cur.p.second == SZ - 1) {
            best_cost = cur.cost;
            break;
        }

        std::array<Bfs_Node, 4> adjacent_nodes {{
            {{cur.p.first + 1, cur.p.second}, cur.cost + 1},
            {{cur.p.first - 1, cur.p.second}, cur.cost + 1},
            {{cur.p.first, cur.p.second + 1}, cur.cost + 1},
            {{cur.p.first, cur.p.second - 1}, cur.cost + 1},
        }};
        for (auto const &node: adjacent_nodes) {
            if (!in_bounds(node.p) || grid[node.p.first][node.p.second] == '#' || visited_map[node.p.first].test(node.p.second)) {
                continue;
            }
            visited_map[node.p.first].set(node.p.second);
            q.push(node);
        }
    }
    return best_cost;
}

std::pair<int32_t, int32_t> solve_p2() {
    int32_t x, y;
    std::string l;
    int32_t counter {};
    while (bfs() != std::numeric_limits<int32_t>::max()) {
        std::cin >> l;
        auto pos {l.find_first_of(',')};
        y = std::stoi(l.substr(0, pos));
        x = std::stoi(l.substr(pos + 1));
        grid[x][y] = '#';
        counter++;
    }
    std::cout << "It took " << counter << " bytes to block the exit\n";
    return {y, x};
}

int main() {
    for (auto &row: grid) {
        row.fill('.');
    }

    std::string l;
    int32_t x, y;
    for (int i = 0; i < BYTES; i++) {
        std::cin >> l;
        auto pos = l.find_first_of(',');
        x = std::stoi(l.substr(0, pos));
        y = std::stoi(l.substr(pos + 1));
        grid[y][x] = '#';
    }
    for (auto const &row: grid) {
        for (auto const &ch: row) {
            std::cout << ch;
        }
        std::cout << "\n";
    }
    auto best_cost {bfs()};
    std::cout << best_cost << "\n";
    auto res {solve_p2()};
    std::cout << res.first << ", " << res.second << "\n";
}
