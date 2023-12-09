#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>

#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/getlines.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

enum class Object { Rock = 0, Paper = 1, Scissors = 2 };
enum class Play { Lose = 0, Draw = 1, Win = 2 };

constexpr std::array<std::array<std::uint32_t, 3>, 3> WINS{{{3, 6, 0}, {0, 3, 6}, {6, 0, 3}}};
constexpr std::array<std::array<Object, 3>, 3>        PLAYS{
    {{Object::Scissors, Object::Rock, Object::Paper},
     {Object::Rock, Object::Paper, Object::Scissors},
     {Object::Paper, Object::Scissors, Object::Rock}}};

std::uint32_t score(std::pair<Object, Object> play) {
    return std::to_underlying(play.second) + 1 +
           WINS[std::to_underlying(play.first)][std::to_underlying(play.second)];
}

int main(int argc, char** argv) {
    assert(argc == 2);

    std::ifstream file{argv[1]};
    auto const    lines = ranges::getlines(file) | ranges::to_vector;

    std::cout << ranges::accumulate(
                     lines | ranges::views::transform([](auto const line) {
                         return std::pair{Object{line[0] - 'A'}, Object{line[2] - 'X'}};
                     }) | ranges::views::transform(score),
                     0)
              << "\n";

    std::cout << ranges::accumulate(
                     lines | ranges::views::transform([](auto const line) {
                         return std::pair{Object{line[0] - 'A'}, Play{line[2] - 'X'}};
                     }) | ranges::views::transform([](auto const plan) {
                         return std::pair{plan.first, PLAYS[std::to_underlying(plan.first)]
                                                           [std::to_underlying(plan.second)]};
                     }) | ranges::views::transform(score),
                     0)
              << "\n";
}
