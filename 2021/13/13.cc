#include <cstdio>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

namespace std {

template <>
struct hash<pair<size_t, size_t>> {
    size_t operator()(pair<size_t, size_t> const& p) const {
        return hash<size_t> {}(p.first) ^ hash<size_t> {}(p.second);
    }
};

} // namespace std

template <size_t axis>
void fold(unordered_set<pair<size_t, size_t>>& points, size_t line) {
    unordered_set<pair<size_t, size_t>> to_add;
    unordered_set<pair<size_t, size_t>> to_remove;
    for (auto& point : points) {
        if (get<axis>(point) <= line)
            continue;
        to_remove.insert(point);
        auto p       = point;
        get<axis>(p) = 2 * line - get<axis>(point);
        to_add.insert(p);
    }

    for (auto& old_point : to_remove)
        points.erase(old_point);
    points.merge(std::move(to_add));
}

int main(int, char* argv[]) {
    ifstream in { argv[1] };

    unordered_set<pair<size_t, size_t>> points;
    string                              line;
    while (getline(in, line)) {
        if (!line.size())
            break;

        size_t pos = line.find(',');
        points.emplace(stoull(line.substr(0, pos)), stoull(line.substr(pos + 1, line.size())));
    }

    size_t i = 0;
    while (getline(in, line)) {
        size_t pos = line.find('=');
        size_t n   = stoull(line.substr(pos + 1, line.size()));

        if (line[pos - 1] == 'x')
            fold<0>(points, n);
        else
            fold<1>(points, n);

        if (i++ == 0)
            printf("%zu\n", points.size());
    }

    size_t max_x = 0;
    size_t max_y = 0;

    for (auto const& point : points) {
        max_x = max(max_x, point.first);
        max_y = max(max_y, point.second);
    }

    vector<vector<bool>> grid;
    for (size_t i = 0; i <= max_y; i++)
        grid.emplace_back(max_x + 1);

    for (auto const& point : points)
        grid[point.second][point.first] = true;

    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++)
            printf(grid[i][j] ? "#" : ".");
        printf("\n");
    }
}
