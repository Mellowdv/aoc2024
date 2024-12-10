#include <bits/stdc++.h>
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);

    std::vector<std::vector<int>> reports;
    std::vector<std::vector<int>> unsafe_reports;
    std::string report, storage;
    while (std::getline(std::cin, report)) {
        std::stringstream ss {report};
        reports.push_back({});
        while (std::getline(ss, storage, ' ')) {
            reports.back().push_back(std::stoi(storage));
        }
    }

    auto check_report = [] (std::vector<int> const &r, int index_to_ignore) {

        bool increasing_safe = true;
        bool decreasing_safe = true;
        int previous, current;
        int diff = 0;

        int starting_index = 0;
        if (index_to_ignore == 0) {
            starting_index = 1;
        }
        current = r[starting_index];
        for (int i = starting_index + 1; i < r.size(); i++) {
            if (i == index_to_ignore) continue;
            previous = current;
            current = r[i];
            diff = std::abs(current - previous);
            // Both increasing and decreasing need to respect this condition
            if (diff < 1 || diff > 3) {
                increasing_safe = false;
                decreasing_safe = false;
                break;
            }

            // Check increasing
            if (increasing_safe) {
                if (current <= previous) {
                    increasing_safe = false;
                }
            }

            // Check decreasing
            if (decreasing_safe) {
                if (current >= previous) {
                    decreasing_safe = false;
                }
            }
        }
        if (increasing_safe || decreasing_safe) {
            return true;
        } else {
            return false;
        }
    };

    int safe_counter = 0;
    for (auto const &report: reports) {
        // Collect unsafe reports for p2
        if (check_report(report, -1)) {
            safe_counter++;
        } else {
            unsafe_reports.push_back(report);
        }
    }
    std::cout << safe_counter << "\n";

    // Part 2 - go over unsafe reports, try to brute force fix them by ignoring 1 element at a time
    for (auto const &unsafe: unsafe_reports) {
        for (int i = 0; i < unsafe.size(); i++) {
            if (check_report(unsafe, i)) {
                safe_counter++;
                break;
            }
        }
    }
    std::cout << safe_counter << "\n";
}
