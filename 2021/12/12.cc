#include <algorithm>
#include <cstdio>
#include <fstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

size_t paths_from(unordered_multimap<string_view, string_view> const& graph,
                  unordered_set<string_view> visited, string_view vertex) {
    if (islower(vertex[0]))
        visited.insert(vertex);
    size_t count      = 0;
    auto [start, end] = graph.equal_range(vertex);
    for (auto it = start; it != end; ++it) {
        auto target = it->second;
        if (target == "end") {
            count++;
            continue;
        }

        if (islower(target[0]) && visited.contains(target))
            continue;

        count += paths_from(graph, visited, target);
    }

    return count;
}

size_t paths_from2(unordered_multimap<string_view, string_view> const& graph,
                   unordered_map<string_view, size_t> visited, string_view vertex,
                   bool revisited = false) {
    size_t count = 0;
    if (islower(vertex[0]) && vertex != "start") {
        auto v = visited.find(vertex);
        if (v != visited.end()) {
            if (revisited)
                return 0;
            revisited = true;
            v->second++;
        } else {
            visited.insert(make_pair(vertex, 1));
        }
    }
    auto [start, end] = graph.equal_range(vertex);
    for (auto it = start; it != end; ++it) {
        auto target = it->second;
        if (target == "start")
            continue;
        if (target == "end") {
            count++;
            continue;
        }

        count += paths_from2(graph, visited, target, revisited);
    }

    return count;
}

int main(int, char* argv[]) {
    ifstream in { argv[1] };

    unordered_multimap<string_view, string_view> edges;
    unordered_set<string>                        vertices;
    string                                       line;
    while (getline(in, line)) {
        size_t pos = line.find('-');
        string _a  = line.substr(0, pos);
        vertices.insert(_a);
        string_view a { *vertices.find(_a) };

        string _b = line.substr(pos + 1, line.size());
        vertices.insert(_b);
        string_view b { *vertices.find(_b) };

        edges.insert(make_pair(a, b));
        edges.insert(make_pair(b, a));
    }

    printf("%zu\n", paths_from(edges, { "start" }, "start"));
    printf("%zu\n", paths_from2(edges, { make_pair("start", 1) }, "start"));
    return 0;
}
