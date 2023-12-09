#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>

#include <range/v3/action/sort.hpp>
#include <range/v3/action/take.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/getlines.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

int main(int argc, char** argv) {
    assert(argc >= 2);

    std::ifstream file{argv[1]};
    auto const    top_count = argc > 2 ? std::atol(argv[2]) : 1;

    std::cout << ranges::accumulate(
                     ranges::getlines(file) | ranges::views::split("") |
                         ranges::views::transform([](auto elf) {
                             return ranges::accumulate(
                                 ranges::views::transform(
                                     elf,
                                     [](auto const food) {
                                         return std::stoul(std::string{food.begin(), food.end()});
                                     }),
                                 0);
                         }) |
                         ranges::to_vector | ranges::actions::sort(std::greater{}) |
                         ranges::actions::take(top_count),
                     0)
              << "\n";
}
