// Not pretty but works
#include <bits/stdc++.h>
int main() {
    std::string input {};
    input.reserve(20'000);

    std::cin >> input;
    int sum_of_free_space {};
    int sum_of_file_space {};
    int number_of_files {};
    for (int i = 0; i < input.size(); i++) {
        if (i % 2 == 1) {
            sum_of_free_space += (input[i] - '0');
        } else {
            sum_of_file_space += (input[i] - '0');
            number_of_files++;
        }
    }
    std::string input_p2 {input};

    // Lots of tracking to do...
    std::vector<int> rearranged(sum_of_file_space);
    int l_file = 0;
    int r_file = number_of_files - 1;
    int l_index = 0;
    int r_index = input.size() - 1;
    int current_index_count, empty_space_count, rearranged_index;
    int converted_files {};
    // Write files on even numbered blocks from the left as is
    // Write files on even numbered blocks from the right into first available gap
    // Keep track of remaining gap and file
    rearranged_index = 0;
    while (converted_files < number_of_files) {
        if (l_index % 2 == 0) {
            current_index_count = input[l_index] - '0';
            for (int j = rearranged_index; j < rearranged_index + current_index_count; j++) {
                rearranged[j] = l_file;
            }
            rearranged_index += current_index_count;
            l_file++;
            l_index++;
            converted_files++;
        } else {
            current_index_count = input[r_index] - '0';
            empty_space_count = input[l_index] - '0';
            int limit = std::min(current_index_count, empty_space_count);
            for (int j = rearranged_index; j < rearranged_index + limit; j++) {
                rearranged[j] = r_file;
            }
            rearranged_index += limit;
            if (empty_space_count >= current_index_count) {
                input[l_index] -= current_index_count;
                r_file--;
                r_index -= 2;
                converted_files++;
            } else {
                input[r_index] -= empty_space_count;
                l_index++;
            }
        }
    }
    int64_t checksum {0};
    for (int k = 0; k < rearranged.size(); k++) {
        checksum += k * rearranged[k];
    }
    std::cout << checksum << "\n";

    // P2
    // Build rearranged array with gaps in it, collect gap information and store in vector
    rearranged.clear();
    rearranged.reserve(sum_of_free_space + sum_of_file_space);
    converted_files = 0;
    l_file = 0;
    r_file = number_of_files - 1;
    l_index = 0;
    r_index = input_p2.size() - 1;
    rearranged_index = 0;
    std::vector<std::pair<int, int>> gaps {};
    while (l_index < input_p2.size() && converted_files < number_of_files) {
        current_index_count = input_p2[l_index] - '0';
        for (int j = rearranged_index; j < rearranged_index + current_index_count; j++) {
            rearranged.push_back(l_index % 2 == 0 ? l_file : -1);
        }
        if (l_index % 2 != 0) {
            gaps.push_back({rearranged_index, current_index_count});
        }
        rearranged_index += current_index_count;
        l_file += l_index % 2 == 0 ? 1 : 0;
        l_index++;
        converted_files += l_index % 2 == 0 ? 1 : 0;
    }

    // Iterate backwards, globbing entire files and looking for a gap
    // If gap found - fill, if not, next file
    r_index = rearranged.size() - 1;
    while (r_index > 0) {
        if (rearranged[r_index] == -1) {
            r_index--;
            continue;
        }
        auto file_end = r_index;
        while (r_index > 0 && rearranged[r_index] == rearranged[r_index - 1]) {
            r_index--;
        }
        auto file_begin = r_index;
        auto file_size = file_end - file_begin + 1;
        r_index = file_begin - 1;
        for (auto &g: gaps) {
            if (g.first < file_begin && g.second >= file_size) {
                int &in_gap_idx = g.first;
                for (int i = file_begin; i < file_begin + file_size; i++) {
                    rearranged[in_gap_idx] = rearranged[i];
                    rearranged[i] = -1;
                    in_gap_idx++;
                }
                g.second -= file_size;
                break;
            } else if (g.first > file_begin) {
                break;
            }
        }
    }
    checksum = 0;
    for (int k = 0; k < rearranged.size(); k++) {
        if (rearranged[k] == -1) continue;
        checksum += k * rearranged[k];
    }
    std::cout << checksum << "\n";
}
