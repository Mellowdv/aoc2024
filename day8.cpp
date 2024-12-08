// Manhattan distance super useful
#include <bits/stdc++.h>
constexpr int64_t SZ = 50;

struct antinode_hash {
    
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

static std::unordered_map<char, std::vector<std::pair<int, int>>> antennas;
static std::unordered_set<std::pair<int, int>, antinode_hash> antinodes_p1;
static std::unordered_set<std::pair<int, int>, antinode_hash> antinodes_p2;

bool in_bounds(std::pair<int, int> const &p) {
    return (0 <= p.first && p.first < SZ &&
            0 <= p.second && p.second < SZ);
}

int main() {
    std::string line;
    int64_t x, y;
    x = y = 0;
    while (std::getline(std::cin, line)) {
        for (auto const &ch: line) {
            if (ch != '.') {
                antennas[ch].push_back({x, y});
            }
            y++;
        }
        x++;
        y = 0;
    }

    int64_t distance;
    int64_t x_distance, y_distance;
    // Loop is a bit messy but it works
    for (auto const &[antenna_type, antenna_list]: antennas) {
        // For every antenna type, calculate the distances between all antennas
        // Try to fit antinodes onto the grid, if within bounds, set insert
        for (int i = 0; i < antenna_list.size(); i++) {
            auto const &a1 = antenna_list[i];
            for (int j = i + 1; j < antenna_list.size(); j++) {
                auto const &a2 = antenna_list[j];
                x_distance = std::abs(a1.first - a2.first);
                y_distance = std::abs(a1.second - a2.second);
                distance = x_distance + y_distance;
                int64_t doubled_x_distance = x_distance << 1;
                int64_t doubled_y_distance = y_distance << 1;
                
                auto t_p = a1.first < a2.first ? a1 : a2;
                auto b_p = a1.first < a2.first ? a2 : a1;

                int64_t top_diff = t_p.second < b_p.second ?
                    doubled_y_distance : -doubled_y_distance;
                int64_t bot_diff = b_p.second > t_p.second ?
                    -doubled_y_distance : doubled_y_distance;
                t_p.first += doubled_x_distance;
                t_p.second += top_diff;
                if (in_bounds(t_p)) antinodes_p1.insert(t_p);

                b_p.first -= doubled_x_distance;
                b_p.second += bot_diff;
                if (in_bounds(b_p)) antinodes_p1.insert(b_p);

                // P2
                // Place antinodes every (distance between two antennas)
                // Check if in bounds
                t_p = a1.first < a2.first ? a1 : a2;
                b_p = a1.first < a2.first ? a2 : a1;
                top_diff = t_p.second < b_p.second ? -y_distance : y_distance;
                bot_diff = b_p.second < t_p.second ? -y_distance : y_distance;
                while (in_bounds(t_p)) {
                    antinodes_p2.insert(t_p);
                    t_p.first -= x_distance;
                    t_p.second += top_diff;
                }
                while (in_bounds(b_p)) {
                    antinodes_p2.insert(b_p);
                    b_p.first += x_distance;
                    b_p.second += bot_diff;
                }
            }
        }
    }
    std::cout << antinodes_p1.size() << "\n";
    std::cout << antinodes_p2.size() << "\n";
}
