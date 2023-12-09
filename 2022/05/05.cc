#include <cassert>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <regex>
#include <string>
#include <tuple>

#include <range/v3/action/split.hpp>
#include <range/v3/algorithm/copy.hpp>
#include <range/v3/algorithm/reverse_copy.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>

int main(int argc, char** argv) {
    assert(argc >= 2);

    std::ifstream file{argv[1]};
    std::string   data{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};
    bool const    p2 = argc > 2 && *argv[2] == '2';

    auto const split = data.find("\n\n");

    auto const stack_data = data.substr(0, split) | ranges::actions::split('\n');
    auto const moves      = data.substr(split + 2) | ranges::actions::split('\n');

    std::map<std::size_t, std::vector<char>> stacks;
    for (auto line :
         stack_data | ranges::views::reverse | ranges::views::transform(ranges::views::enumerate)) {
        for (auto [index, elem] :
             ranges::views::filter(line, [](auto elem) { return std::isalpha(elem.second); })) {
            stacks[index / 4].push_back(elem);
        }
    }

    std::regex move{R"(move (\d+) from (\d+) to (\d+))"};
    for (auto [n, from_index, to_index] :
         moves | ranges::views::transform([&move](auto const line) {
             std::smatch match;
             std::regex_match(line, match, move);

             return std::tuple{std::stoul(match[1].str()), std::stoul(match[2].str()),
                               std::stoul(match[3].str())};
         })) {
        auto& from = stacks[from_index - 1];
        auto& to   = stacks[to_index - 1];

        auto top_n = ranges::views::reverse(from) | ranges::views::take(n);
        if (!p2) {
            ranges::copy(top_n, std::back_inserter(to));
        } else {
            ranges::reverse_copy(top_n, std::back_inserter(to));
        }
        from.resize(from.size() - n);
    }

    std::cout << (stacks |
                  ranges::views::transform([](auto const stack) { return stack.second.back(); }) |
                  ranges::to<std::string>)
              << "\n";
}
