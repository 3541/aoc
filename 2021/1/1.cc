#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

using namespace std;

ssize_t increases(vector<ssize_t> const& v) {
    vector<ssize_t> diff;
    adjacent_difference(v.begin(), v.end(), back_inserter(diff));

    return accumulate(diff.begin(), diff.end(), 0,
                      [](ssize_t acc, ssize_t d) { return acc + (d > 0); }) -
           1;
}

ssize_t part_1(vector<ssize_t> const& lines) { return increases(lines); }

ssize_t part_2(vector<ssize_t> const& lines) {
    vector<ssize_t> windows(lines.size() - 2);
    for (size_t i = 2; i < lines.size(); i++)
        windows[i - 2] = lines[i - 2] + lines[i - 1] + lines[i];
    return increases(windows);
}

int main() {
    ifstream        in { "data" };
    vector<ssize_t> lines;

    copy(istream_iterator<ssize_t> { in }, istream_iterator<ssize_t> {}, back_inserter(lines));

    printf("Part 1: %zu.\nPart 2: %zu.\n", part_1(lines), part_2(lines));

    return 0;
}
