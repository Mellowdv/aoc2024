#include <bits/stdc++.h>
constexpr int SZ = 140;
constexpr int DIAGONALS = SZ * 2 - 1;

static std::array<std::array<char, SZ>, SZ> rows;
static std::array<std::array<char, SZ>, SZ> columns;
static std::array<std::array<char, SZ>, DIAGONALS> left_diagonals;
static std::array<std::array<char, SZ>, DIAGONALS> right_diagonals;
static std::string fwd {"XMAS"};
static std::string bwd {"SAMX"};

int count_xmases(std::array<char, SZ> const &to_count) {
    int index, count;
    index = count = 0;
    int fwd_idx, bwd_idx;
    fwd_idx = bwd_idx = 0;
    while (index < SZ) {
        if (to_count[index] == fwd[fwd_idx]) {
            if (fwd[fwd_idx] == 'S') {
                fwd_idx = 0;
                count++;
            } else {
                fwd_idx++;
            }
        } else {
            fwd_idx = to_count[index] == 'X' ? 1 : 0;
        }

        if (to_count[index] == bwd[bwd_idx]) {
            if (bwd[bwd_idx] == 'X') {
                bwd_idx = 0;
                count++;
            } else {
                bwd_idx++;  
            }
        } else {
            bwd_idx = to_count[index] == 'S' ? 1 : 0;
        }
        index++;
    }
    return count;
}

int count_x_mases() {
    int count = 0;
    for (int i = 1; i < SZ - 1; i++) {
        for (int j = 1; j < SZ - 1; j++) {
            if (rows[i][j] == 'A') {
                if (
                     (
                       (
                         rows[i - 1][j - 1] == 'M' &&
                         rows[i + 1][j + 1] == 'S'
                       ) 
                       ||
                       (
                         rows[i - 1][j - 1] == 'S' &&
                         rows[i + 1][j + 1] == 'M'
                       )
                     ) 
                     &&
                     (
                       (
                         rows[i - 1][j + 1] == 'M' &&
                         rows[i + 1][j - 1] == 'S'
                       )
                       ||
                       (
                         rows[i - 1][j + 1] == 'S' &&
                         rows[i + 1][j - 1] == 'M'
                       )
                     )
                   ) 
                {
                    count++;
                }
            }
        }
    }
    return count;
}

int main() {

    std::string line;
    int row_idx, col_idx, diagonal_idx;
    row_idx = col_idx = diagonal_idx = 0;
    int32_t index = 0;
    while (std::getline(std::cin, line)) {
        index = 0;
        diagonal_idx = 0;
        for (int i = 0; i < SZ; i++) {
            char ch = line[i];
            diagonal_idx = index + row_idx >= SZ ? diagonal_idx - 1 : row_idx;

            rows[row_idx][index] = ch;
            columns[index][row_idx] = ch;
            left_diagonals[row_idx + index][diagonal_idx] = ch;
            right_diagonals[row_idx + index][diagonal_idx] = line[SZ - 1 - index];
            index++;
        }
        row_idx++;
    }
    int sum = 0;
    for (auto const &r: rows) {
        sum += count_xmases(r);
    }
    for (auto const &r: columns) {
        sum += count_xmases(r);
    }
    for (auto const &r: left_diagonals) {
        sum += count_xmases(r);
    }
    for (auto const &r: right_diagonals) {
        sum += count_xmases(r);
    }
    std::cout << sum << "\n";
    std::cout << count_x_mases() << "\n";
}
