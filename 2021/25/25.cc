#include <cstdio>
#include <fstream>
#include <vector>

using namespace std;

struct Map {
    struct Row {
        vector<uint8_t>& row;

        uint8_t& operator[](size_t i) { return row[i % row.size()]; }
    };

    vector<vector<uint8_t>> map;

    Row operator[](size_t i) { return Row { map[i % map.size()] }; }

    void print() {
        for (auto& row : map) {
            for (auto v : row)
                printf("%c", v);
            printf("\n");
        }
        printf("\n");
    }

    bool step() {
        bool  moved = false;
        auto& that  = *this;
        Map   new_map { map };

        for (size_t i = 0; i < map.size(); i++) {
            for (size_t j = 0; j < map[i].size(); j++) {
                if (that[i][j] == '>' && that[i][j + 1] == '.') {
                    new_map[i][j]     = '.';
                    new_map[i][j + 1] = '>';
                    moved             = true;
                }
            }
        }

        map     = move(new_map.map);
        new_map = Map { map };

        for (size_t i = 0; i < map.size(); i++) {
            for (size_t j = 0; j < map[i].size(); j++) {
                if (that[i][j] == 'v' && that[i + 1][j] == '.') {
                    new_map[i][j]     = '.';
                    new_map[i + 1][j] = 'v';
                    moved             = true;
                }
            }
        }

        map = move(new_map.map);

        return moved;
    }
};

int main(int, char* argv[]) {
    ifstream in { argv[1] };

    Map    map;
    string line;
    while (getline(in, line)) {
        vector<uint8_t> row;

        for (char c : line)
            row.push_back(c);

        map.map.push_back(move(row));
    }

    size_t step = 1;
    while (map.step())
        printf("\r%zu", step++);

    printf("\r%zu\n", step);
}
