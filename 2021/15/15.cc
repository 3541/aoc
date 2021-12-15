#include <cstdio>
#include <fstream>
#include <limits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

constexpr ssize_t DELTAS[4][2] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };

namespace std {

template <>
struct hash<pair<size_t, size_t>> {
    size_t operator()(pair<size_t, size_t> const& p) const {
        return hash<size_t> {}(p.first) ^ hash<size_t> {}(p.second);
    }
};

} // namespace std

uint8_t get_risk(vector<vector<uint8_t>> const& grid, size_t r, size_t c, bool p1 = true) {
    if (p1)
        return grid[r][c];

    uint8_t ret  = grid[r % grid.size()][c % grid[0].size()];
    uint8_t diff = r / grid.size() + c / grid[0].size();

    return (ret + diff - 1) % 9 + 1;
}

size_t dijkstra(vector<vector<uint8_t>> const& grid, bool p1 = true) {
    auto                                        pos      = make_pair<size_t, size_t>(0, 0);
    size_t                                      pos_dist = grid[0][0];
    unordered_set<pair<size_t, size_t>>         visited;
    unordered_map<pair<size_t, size_t>, size_t> unvisited;

    size_t rows = p1 ? grid.size() : grid.size() * 5;
    size_t cols = p1 ? grid[0].size() : grid[0].size() * 5;
    auto   dest = make_pair(rows - 1, cols - 1);
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            if (i != 0 || j != 0)
                unvisited.insert(make_pair(make_pair(i, j), std::numeric_limits<size_t>::max()));
    while (true) {
        for (auto [dr, dc] : DELTAS) {
            ssize_t nr = pos.first + dr;
            ssize_t nc = pos.second + dc;

            if (nr < 0 || nc < 0 || nr >= static_cast<ssize_t>(rows) ||
                nc >= static_cast<ssize_t>(cols))
                continue;

            auto neighbor = make_pair<size_t, size_t>(nr, nc);
            if (visited.contains(neighbor))
                continue;

            auto neighbor_dist = unvisited.find(neighbor)->second;
            neighbor_dist =
                min(neighbor_dist, pos_dist + get_risk(grid, neighbor.first, neighbor.second, p1));
            unvisited.insert_or_assign(neighbor, neighbor_dist);
        }

        visited.insert(pos);
        pos      = min_element(unvisited.begin(), unvisited.end(), [](auto e1, auto e2) {
                  return e1.second < e2.second;
              })->first;
        pos_dist = unvisited.find(pos)->second;
        if (pos == dest)
            return pos_dist - grid[0][0];
        unvisited.erase(pos);
    }
}

int main(int argc, char* argv[]) {
    ifstream in { argv[1] };

    vector<vector<uint8_t>> grid;
    string                  line;
    while (getline(in, line)) {
        vector<uint8_t> row;

        for (char c : line)
            row.push_back(c - '0');

        grid.push_back(move(row));
    }

    printf("%zu\n", dijkstra(grid));
    printf("%zu\n", dijkstra(grid, false));
}
