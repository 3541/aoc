#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <utility>

#include <range/v3/action/sort.hpp>
#include <range/v3/algorithm/set_algorithm.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/getlines.hpp>
#include <range/v3/view/transform.hpp>

std::uint32_t score(char c) { return 'A' <= c && c <= 'Z' ? c - 'A' + 27 : c - 'a' + 1; }

int main(int argc, char** argv) {
    assert(argc == 2);

    std::ifstream file{argv[1]};
    auto const    lines = ranges::getlines(file) | ranges::to_vector;

    std::cout << ranges::accumulate(
                     lines | ranges::views::transform([](auto const line) {
                         char diff;
                         return *std::prev(ranges::set_intersection(
                             line.substr(0, line.length() / 2) | ranges::actions::sort,
                             line.substr(line.length() / 2) | ranges::actions::sort, &diff));
                     }) | ranges::views::transform(score),
                     0)
              << "\n";

    std::cout << ranges::accumulate(
                     lines | ranges::views::chunk(3) | ranges::views::transform([](auto group) {
                         return group | ranges::views::transform([](auto line) {
                                    return line | ranges::to_vector | ranges::actions::sort;
                                });
                     }) | ranges::views::transform([](auto group) {
                         std::vector<char> diff;

                         ranges::set_intersection(group[0], group[1], std::back_inserter(diff));
                         char badge;
                         return *std::prev(ranges::set_intersection(diff, group[2], &badge));
                     }) | ranges::views::transform(score),
                     0)
              << "\n";
}
