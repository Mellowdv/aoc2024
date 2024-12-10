#include <bits/stdc++.h>
int main() {
    std::string line;
    std::getline(std::cin, line); // Input manually concatenated into one line before running
    std::regex r("mul\\(([0-9]+),([0-9]+)\\)", std::regex::ECMAScript);

    auto match_mul_and_calc =
        [&line, &r] (size_t start_idx, size_t end_idx) {
        int64_t lhs, rhs, sum;
        sum = 0;
        std::sregex_token_iterator it {line.begin() + start_idx, line.begin() + end_idx,
                                   r,
                                   {1, 2}};
        std::sregex_token_iterator end {};
        while (it != end) {
            lhs = std::stoll(*it);
            it++;
            rhs = std::stoll(*it);
            it++;
            sum += lhs * rhs;
        }
        return sum;
    };

    std::cout << line.size() << "\n";
    int64_t sum = match_mul_and_calc(0, line.size());
    std::cout << sum << "\n";

    // Split the string into don't() ... do() chunks and run the same regex, input small enough
    auto chunk_start = line.find("don't()");
    auto chunk_end = line.find("do()", chunk_start);
    while (chunk_start != std::string::npos) {
        sum -= match_mul_and_calc(chunk_start, chunk_end);

        chunk_start = line.find("don't()", chunk_end);
        chunk_end = line.find("do()", chunk_start);
        if (chunk_end == std::string::npos) {
            chunk_end = line.size();
        }
    }
    std::cout << sum << "\n";
}
