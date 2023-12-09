#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <regex>
#include <utility>

#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/getlines.hpp>
#include <range/v3/view/transform.hpp>

bool contains(std::pair<std::size_t, std::size_t> a, std::pair<std::size_t, std::size_t> b) {
    return (a.first <= b.first && a.second >= b.second) ||
           (a.first >= b.first && a.second <= b.second);
}

bool overlaps(std::pair<std::size_t, std::size_t> a, std::pair<std::size_t, std::size_t> b) {
    return a.first <= b.second && a.second >= b.first;
}

int main(int argc, char** argv) {
    assert(argc == 2);

    std::ifstream file{argv[1]};
    std::regex    ranges{R"((\d+)-(\d+),(\d+)-(\d+))"};

    auto pairs =
        ranges::getlines(file) | ranges::views::transform([&ranges](auto const line) {
            std::smatch match;
            std::regex_match(line, match, ranges);

            return std::pair{std::pair{std::stoul(match[1].str()), std::stoul(match[2].str())},
                             std::pair{std::stoul(match[3].str()), std::stoul(match[4].str())}};
        });

    std::size_t subsets     = 0;
    std::size_t overlapping = 0;
    for (auto const [a, b] : pairs) {
        subsets += contains(a, b);
        overlapping += overlaps(a, b);
    }

    std::cout << subsets << "\n";
    std::cout << overlapping << "\n";
}
