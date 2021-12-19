#include <fstream>
#include <functional>
#include <tuple>
#include <unordered_set>
#include <vector>
#include <iostream>

using namespace std;

using Point = array<ssize_t, 3>;

namespace std {

template <>
struct hash<Point> {
    size_t operator()(Point const& p) const {
        return hash<ssize_t> {}(p[0]) ^ hash<ssize_t> {}(p[1]) ^ hash<ssize_t> {}(p[2]);
    }
};

} // namespace std

Point operator-(Point const& p1, Point const& p2) {
    return { p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2] };
}

Point operator+(Point const& p1, Point const& p2) {
    return { p1[0] + p2[0], p1[1] + p2[1], p1[2] + p2[2] };
}

size_t dist(Point const& p1, Point const& p2) {
    auto diff = p1 - p2;
    return abs(diff[0]) + abs(diff[1]) + abs(diff[2]);
}

int main(int argc, char* argv[]) {
    ifstream in { argv[1] };

    vector<unordered_set<Point>> scanners;
    string                       line;
    while (getline(in, line)) {
        // Skip header line;
        unordered_set<Point> points;

        for (getline(in, line); !line.empty(); getline(in, line)) {
            size_t  c1 = line.find(",");
            size_t  c2 = line.find(",", c1 + 1);
            ssize_t x  = stoll(line.substr(0, c1));
            ssize_t y  = stoll(line.substr(c1 + 1, c2));
            ssize_t z  = stoll(line.substr(c2 + 1));

            points.insert({ x, y, z });
        }

        scanners.push_back(move(points));
    }

    vector<function<Point(Point const&)>> rotations;
    for (size_t i = 0; i < 6; i++) {
        for (size_t j = 0; j < 6; j++) {
            if (j % 3 == i % 3)
                continue;
            for (size_t k = 0; k < 6; k++) {
                if (k % 3 == j % 3 || k % 3 == i % 3)
                    continue;

                auto sgn = [](size_t n) { return n >= 3 ? -1 : 1; };

                rotations.push_back([i, j, k, sgn](Point const& p) {
                    return Point { p[i % 3] * sgn(i), p[j % 3] * sgn(j), p[k % 3] * sgn(k) };
                });
            }
        }
    }

    unordered_set<Point> resolved { scanners[0] };
    unordered_set<Point> scanner_pos { { 0, 0, 0 } };
    scanners.erase(scanners.begin());
    while (!scanners.empty()) {
    again:
        cout << scanners.size() << "\r" << flush;
        for (size_t i = 0; i < scanners.size(); i++) {
            auto const& scanner = scanners[i];
            for (auto const& rotation : rotations) {
                for (auto const& r : scanner) {
                    auto ref = rotation(r);

                    for (auto const& other : resolved) {
                        auto                 diff = other - ref;
                        unordered_set<Point> transformed;
                        for (auto const& p : scanner)
                            transformed.insert(rotation(p) + diff);

                        size_t matches = 0;
                        for (auto const& p : transformed)
                            matches += resolved.contains(p);

                        if (matches >= 12) {
                            resolved.merge(transformed);
                            scanners.erase(scanners.begin() + i);
                            scanner_pos.insert(diff);
                            goto again;
                        }
                    }
                }
            }
        }
    }

    cout << resolved.size() << endl;

    size_t max_dist = 0;
    for (auto const& p : scanner_pos)
        for (auto const& q : scanner_pos)
            max_dist = max(max_dist, dist(p, q));

    cout << max_dist << endl;
}
