#include <cassert>
#include <concepts>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

#include <range/v3/action/sort.hpp>
#include <range/v3/action/unique.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range/traits.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/view/transform.hpp>

template <typename R, typename V>
concept RangeOver = ranges::range<R> && std::same_as<V, ranges::range_value_t<R>>;

std::size_t index_of_first_unique_n(RangeOver<char> auto&& range, std::size_t n) {
    return *(range | ranges::views::sliding(n) |
             ranges::views::transform(ranges::to_vector | ranges::actions::sort |
                                      ranges::actions::unique) |
             ranges::views::enumerate |
             ranges::views::filter([n](auto const elem) { return elem.second.size() == n; }) |
             ranges::views::transform([n](auto const elem) { return elem.first + n; }) |
             ranges::views::take(1))
                .begin();
}

int main(int argc, char** argv) {
    assert(argc == 2);

    std::ifstream file{argv[1]};
    std::string   data{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};

    std::cout << index_of_first_unique_n(data, 4) << "\n";
    std::cout << index_of_first_unique_n(data, 14) << "\n";
}
