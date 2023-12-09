#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>

#include <range/v3/algorithm/any_of.hpp>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/iterator/default_sentinel.hpp>
#include <range/v3/iterator/operations.hpp>
#include <range/v3/range/access.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range/traits.hpp>
#include <range/v3/view/all.hpp>
#include <range/v3/view/any_view.hpp>
#include <range/v3/view/chunk.hpp>
#include <range/v3/view/facade.hpp>
#include <range/v3/view/getlines.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/view.hpp>
#include <range/v3/view/zip.hpp>

// From https://github.com/ericniebler/range-v3/blob/master/example/calendar.cpp#L192
template <typename R>
requires ranges::forward_range<ranges::range_value_t<R>>
struct Interleave : public ranges::view_facade<Interleave<R>> {
private:
    friend ranges::range_access;
    struct Cursor;
    using InnerRange = ranges::range_value_t<R>;

    std::vector<InnerRange> m_inner_ranges;

    Cursor begin_cursor();

public:
    Interleave() = default;
    explicit Interleave(R);
};

template <typename R>
requires ranges::forward_range<ranges::range_value_t<R>>
struct Interleave<R>::Cursor {
private:
    std::size_t                                 m_index{0};
    std::vector<InnerRange>*                    m_inner_ranges;
    std::vector<ranges::iterator_t<InnerRange>> m_iterators;

public:
    explicit Cursor(std::vector<InnerRange>&);

    Cursor() = default;

    decltype(auto) read() const;
    void           next();
    bool           equal(ranges::default_sentinel_t) const;
    bool           equal(Cursor const&) const;
};

template <typename R>
requires ranges::forward_range<ranges::range_value_t<R>>
auto Interleave<R>::begin_cursor() -> Cursor { return Cursor{m_inner_ranges}; }

template <typename R>
requires ranges::forward_range<ranges::range_value_t<R>> Interleave<R>::Interleave(R ranges) :
    m_inner_ranges{std::move(ranges) | ranges::to_vector} {}

template <typename R>
requires ranges::forward_range<ranges::range_value_t<R>>
Interleave<R>::Cursor::Cursor(std::vector<InnerRange>& inner_ranges) :
    m_inner_ranges{&inner_ranges},
    m_iterators{*m_inner_ranges | ranges::views::transform(ranges::begin) | ranges::to_vector} {}

template <typename R>
requires ranges::forward_range<ranges::range_value_t<R>>
decltype(auto) Interleave<R>::Cursor::read() const { return *m_iterators[m_index]; }

template <typename R>
requires ranges::forward_range<ranges::range_value_t<R>>
void Interleave<R>::Cursor::next() {
    if ((++m_index %= m_iterators.size()) != 0)
        return;

    for (auto& it : m_iterators)
        ++it;
}

template <typename R>
requires ranges::forward_range<ranges::range_value_t<R>>
bool Interleave<R>::Cursor::equal(ranges::default_sentinel_t) const {
    if (m_index != 0)
        return false;

    return ranges::any_of(
        ranges::views::zip(*m_inner_ranges | ranges::views::transform(ranges::end), m_iterators),
        [](auto const pair) { return pair.first == pair.second; });
}

template <typename R>
requires ranges::forward_range<ranges::range_value_t<R>>
bool Interleave<R>::Cursor::equal(Cursor const& other) const {
    return m_index == other.m_index && m_iterators == other.m_iterators;
}

auto interleave() {
    return ranges::make_view_closure([](auto&& ranges) {
        return Interleave{ranges::views::all(std::forward<decltype(ranges)>(ranges))};
    });
}

auto transpose() {
    return ranges::make_view_closure([](auto&& ranges) {
        return std::forward<decltype(ranges)>(ranges) | interleave() |
               ranges::views::chunk(ranges::distance(ranges));
    });
}

struct Tree {
    std::uint8_t m_height;
    bool         m_visible;
};

void scan_view(auto&& range) {
    for (auto&& row : std::forward<decltype(range)>(range)) {
        std::uint8_t tallest = 0;

        bool edge = true;
        for (auto& tree : row) {
            if (edge)
                tree.m_visible = true;

            if (tree.m_height > tallest)
                tree.m_visible = true;

            tallest = std::max(tallest, tree.m_height);
            if (tallest == 9)
                break;

            edge = false;
        }
    }
}

int main(int argc, char** argv) {
    assert(argc == 2);

    std::ifstream file{argv[1]};

    auto trees =
        ranges::getlines(file) |
        ranges::views::transform(ranges::views::transform([](auto const tree) {
                                     return Tree{static_cast<std::uint8_t>(tree - '0'), false};
                                 }) |
                                 ranges::to_vector) |
        ranges::to_vector;

    scan_view(trees);
    scan_view(trees | ranges::views::transform(ranges::views::reverse));
    scan_view(trees | ranges::views::transform(ranges::views::all) | transpose());
    scan_view(trees | ranges::views::transform(ranges::views::all) | ranges::views::reverse |
              transpose());

    auto const count = ranges::count_if(trees | ranges::views::join, &Tree::m_visible);
    std::cout << count << "\n";

    for (auto const& row : trees) {
        for (auto const& tree : row) {
            std::cout << (tree.m_visible ? "[" : " ") << static_cast<int>(tree.m_height)
                      << (tree.m_visible ? "]" : " ");
        }

        std::cout << "\n";
    }
}
