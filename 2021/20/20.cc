#include <algorithm>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <unordered_set>
#include <vector>

using namespace std;

namespace std {

template <>
struct hash<pair<ssize_t, ssize_t>> {
    ssize_t operator()(pair<ssize_t, ssize_t> const& p) const {
        return hash<ssize_t> {}(p.first) ^ hash<ssize_t> {}(p.second);
    }
};

} // namespace std

struct Image {
    unordered_set<pair<ssize_t, ssize_t>> data;
    bool                                  default_lit = false;

    bool lit(pair<ssize_t, ssize_t> pos) const { return default_lit != data.contains(pos); }
    void set(pair<ssize_t, ssize_t> pos, bool val) {
        if (default_lit != val)
            data.insert(pos);
    }

    uint16_t num_around(pair<ssize_t, ssize_t> pos) const {
        uint16_t ret = 0;
        for (ssize_t i = pos.first - 1; i <= pos.first + 1; i++) {
            for (ssize_t j = pos.second - 1; j <= pos.second + 1; j++) {
                ret <<= 1;
                ret |= lit(make_pair(i, j));
            }
        }

        return ret;
    }

    pair<pair<ssize_t, ssize_t>, pair<ssize_t, ssize_t>> range() const {
        auto yrange = minmax_element(data.begin(), data.end(), [](auto const& e1, auto const& e2) {
            return e1.first < e2.first;
        });
        auto xrange = minmax_element(data.begin(), data.end(), [](auto const& e1, auto const& e2) {
            return e1.second < e2.second;
        });

        return make_pair(make_pair(yrange.first->first, yrange.second->first),
                         make_pair(xrange.first->second, xrange.second->second));
    }

    Image transform(vector<bool> const& reference) {
        auto [yrange, xrange] = range();

        Image ret;
        ret.default_lit = default_lit ? reference.back() : reference.front();

        for (ssize_t i = yrange.first - 1; i <= yrange.second + 1; i++)
            for (ssize_t j = xrange.first - 1; j <= xrange.second + 1; j++)
                ret.set(make_pair(i, j), reference[num_around(make_pair(i, j))]);

        return ret;
    }

    Image transform(vector<bool> const& reference, size_t rounds) {
        assert(rounds > 0);

        Image ret = *this;
        for (size_t i = 0; i < rounds; i++)
            ret = ret.transform(reference);

        return ret;
    }

    void print() {
        auto [yrange, xrange] = range();

        for (ssize_t i = yrange.first; i <= yrange.second; i++) {
            for (ssize_t j = xrange.first; j <= xrange.second; j++) {
                if (lit(make_pair(i, j)))
                    printf("#");
                else
                    printf(".");
            }
            printf("\n");
        }
    }
};

int main(int argc, char* argv[]) {
    ifstream in { argv[1] };

    string ref;
    getline(in, ref);

    vector<bool> reference;
    for (auto c : ref)
        reference.push_back(c == '#');

    // Skip blank line.
    string line;
    getline(in, line);
    assert(line.empty());

    Image image;
    for (ssize_t i = 0; getline(in, line); i++)
        for (ssize_t j = 0; j < line.size(); j++)
            if (line[j] == '#')
                image.set(make_pair(i, j), true);

    auto twice_enhanced = image.transform(reference, 2);
    printf("%zu\n", twice_enhanced.data.size());

    auto fifty_times_enhanced = twice_enhanced.transform(reference, 48);
    printf("%zu\n", fifty_times_enhanced.data.size());
}
