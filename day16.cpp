// Work in progress, clean this dumb shit up...
#include <bits/stdc++.h>
#include <cstdint>
constexpr std::size_t SZ = 141;
static std::array<std::array<char, SZ>, SZ> grid {};

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


enum Direction {
    E = 0,
    W = 1,
    N = 2,
    S = 3
};
struct Move {
    std::pair<int64_t, int64_t> coords;
    int64_t cost;
    Direction dir;
    int64_t branch = 0;
};

struct move_compare {
    bool operator()(Move const &lhs, Move const &rhs) const {
        return lhs.cost > rhs.cost;
    }
};

constexpr std::array<std::array<int64_t, 4>, 4> direction_to_cost {{
    {1, 2001, 1001, 1001},
    {2001, 1, 1001, 1001},
    {1001, 1001, 1, 2001},
    {1001, 1001, 2001, 1}
}};

bool in_bounds(std::pair<int64_t, int64_t> p) {
    return (0 <= p.first && p.first < SZ && 0 <= p.second && p.second < SZ);
}

int64_t dijkstra(std::pair<int64_t, int64_t> start) {
    std::array<std::array<int64_t, SZ>, SZ> cost_map {};
    for (auto &row: cost_map) {
        for (auto &col: row) {
            col = std::numeric_limits<int64_t>::max();
        }
    }

    std::array<std::bitset<SZ>, SZ> visited_map {};
    std::priority_queue<Move, std::vector<Move>, move_compare> q;
    q.push({start, 0, E});
    cost_map[start.first][start.second] = 0;

    while (!q.empty()) {
        auto const current_move = q.top();
        auto const current_coords = current_move.coords;
        q.pop();
        visited_map[current_coords.first].set(current_coords.second);
        std::array<Move, 4> adjacent_points {{
            {{current_coords.first + 1, current_coords.second},
              direction_to_cost[current_move.dir][N], N},
            {{current_coords.first - 1, current_coords.second},
              direction_to_cost[current_move.dir][S], S},
            {{current_coords.first, current_coords.second + 1},
              direction_to_cost[current_move.dir][E], E},
            {{current_coords.first, current_coords.second - 1},
              direction_to_cost[current_move.dir][W], W}
        }};
        std::vector<std::pair<int64_t, int64_t>> path_length {};
        for (auto &p: adjacent_points) {
            const auto &coords {p.coords};
            if (visited_map[coords.first].test(coords.second)) continue;
            if (grid[coords.first][coords.second] == '#') {
                continue;
            }
            auto const cost = direction_to_cost[current_move.dir][p.dir];
            if (cost_map[current_coords.first][current_coords.second] + cost < cost_map[coords.first][coords.second]) {
                cost_map[coords.first][coords.second] = cost_map[current_coords.first][current_coords.second] + cost;
                p.cost = cost_map[current_coords.first][current_coords.second] + cost;
                q.push(p);
            } else {
                continue;
            }
        }
    }
    for (auto const &row: cost_map) {
        for (auto const &col: row) {
            if (col == std::numeric_limits<int64_t>::max()) {
                std::cout << std::setw(8) << -1 << " ";
                continue;
            }
            std::cout << std::setw(8) << col << " ";
        }
        std::cout << "\n";
    }
    return cost_map[1][SZ - 2];
}

std::pair<int64_t, int64_t> direction_to_reverse_delta(Direction d) {
    switch (d) {
        case E:
            return {0, -1};
        case W:
            return {0, 1};
        case N:
            return {1, 0};
        case S:
            return {-1, 0};
        default:
            std::cout << "Bad dir!\n";
            return {0, 0};
    }
}
int64_t dijkstra_3d(std::pair<int64_t, int64_t> start, int64_t p1_result) {
    std::array<std::array<std::array<int64_t, 4>, SZ>, SZ> cost_map {};
    for (auto &row: cost_map) {
        for (auto &col: row) {
            for (auto &dir: col) {
                dir = std::numeric_limits<int64_t>::max();
            }
        }
    }
    std::array<std::array<std::bitset<4>, SZ>, SZ> visited_map {};
    std::array<std::array<std::vector<std::tuple<int64_t, int64_t, Direction>>, SZ>, SZ>  prevs {};

    std::priority_queue<Move, std::vector<Move>, move_compare> q;
    q.push({start, 0, E});
    for (auto &dir: cost_map[start.first][start.second]) {
        dir = 0;
    }

    while (!q.empty()) {
        auto const current_move = q.top();
        auto const current_coords = current_move.coords;
        q.pop();
        std::array<Move, 4> adjacent_points {{
            {{current_coords.first + 1, current_coords.second},
              direction_to_cost[current_move.dir][N], N},
            {{current_coords.first - 1, current_coords.second},
              direction_to_cost[current_move.dir][S], S},
            {{current_coords.first, current_coords.second + 1},
              direction_to_cost[current_move.dir][E], E},
            {{current_coords.first, current_coords.second - 1},
              direction_to_cost[current_move.dir][W], W}
        }};
        for (auto &p: adjacent_points) {
            const auto &coords {p.coords};
            if (grid[coords.first][coords.second] == '#') {
                continue;
            }
            auto const cost = direction_to_cost[current_move.dir][p.dir];
            if (cost == 2001) continue; // This means we're doubling back, pointless
            if (cost_map[current_coords.first][current_coords.second][current_move.dir] + cost < cost_map[coords.first][coords.second][p.dir]) {
                std::cout << "Setting cost for " << coords.first << ", " << coords.second << ", " << p.dir << "; " <<
                    cost_map[current_coords.first][current_coords.second][current_move.dir] + cost << "\n";
                std::cout << "Coming from " << current_coords.first << ", " << current_coords.second << ", " << current_move.dir << "\n";
                cost_map[coords.first][coords.second][p.dir] = cost_map[current_coords.first][current_coords.second][current_move.dir] + cost;
                p.cost = cost_map[current_coords.first][current_coords.second][current_move.dir] + cost;
                q.push(p);
                prevs[coords.first][coords.second].push_back(std::make_tuple(current_coords.first, current_coords.second, p.dir));
            } else {
                continue;
            }
        }
    }

    std::unordered_set<std::pair<int64_t, int64_t>, pair_hash> uniques {};
    std::stack<std::tuple<int64_t, int64_t, Direction>> s {};
    uniques.insert({1, SZ - 2});
    for (auto const &[x, y, d]: prevs[1][SZ - 2]) {
        std::cout << cost_map[x][y][d] << " ";
        if (cost_map[x][y][d] == p1_result - 1 ||
            cost_map[x][y][d] == p1_result - 1001) {
            s.push({x, y, d});
        }
    }
    std::cout << "\n";
    while (!s.empty()) {
        auto const cur = s.top();
        s.pop();

        const auto &[x, y, d] = cur;
        uniques.insert({x, y});
        auto current_cost = cost_map[x][y][d];
        std::cout << "Current cost: " << current_cost << "\n";
        std::cout << "Prevs for " << x << ", " << y << ":\n";
        for (auto const &[nx, ny, nd]: prevs[std::get<0>(cur)][std::get<1>(cur)]) {
            std::cout << nx << ", " << ny << ", " << nd << ", cost: " << cost_map[nx][ny][nd] << "\n";
            int dir_counter = 0;
            for (auto const item: cost_map[nx][ny]) {
                std::cout << "Found cost " << item << "\n";
                if (cost_map[x][y][d] - 1001 == item ||
                    cost_map[x][y][d] - 1 == item) {
                    s.push({nx, ny, Direction(dir_counter)});
                }
                dir_counter++;
            }
        }
        std::cout << "\n";
    }
    std::cout << uniques.size() << "\n";
    return 0;
}

int main() {
    std::string l;
    std::pair<int64_t, int64_t> start;
    for (int i = 0; i < SZ; i++) {
        std::cin >> l;
        for (int j = 0; j < SZ; j++) {
            grid[i][j] = l[j];
            if (grid[i][j] == 'S') {
                start = {i, j};
            }
        }
    }
    for (auto const &row: grid) {
        for (auto const &ch: row) {
            std::cout << ch;
        }
        std::cout << "\n";
    }
    std::cout << start.first << ", " << start.second << "\n";
    int64_t p1_result = dijkstra(start);
    std::cout << p1_result << "\n";
    int64_t p2_result = dijkstra_3d(start, p1_result);
}
