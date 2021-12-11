#include <cstdio>
#include <fstream>
#include <limits>
#include <vector>

using namespace std;

constexpr uint8_t FLASHED      = numeric_limits<uint8_t>::max();
constexpr ssize_t DELTAS[8][2] = { { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 },
                                   { 0, 1 },   { 1, -1 }, { 1, 0 },  { 1, 1 } };

int main(int, char** argv) {
    ifstream in { argv[1] };

    vector<vector<uint8_t>> grid;

    string line;
    while (getline(in, line)) {
        vector<uint8_t> row;
        for (char c : line)
            row.push_back(c - '0');

        grid.push_back(move(row));
    }

    size_t flashes   = 0;
    size_t all_flash = 0;
    for (size_t step = 0; ; step++) {
        for (auto& row : grid)
            for (auto& octopus : row)
                octopus++;

        bool flashing = true;
        while (flashing) {
            flashing = false;
            for (size_t r = 0; r < grid.size(); r++) {
                for (size_t c = 0; c < grid[r].size(); c++) {
                    if (grid[r][c] <= 9 || grid[r][c] == FLASHED)
                        continue;

                    grid[r][c] = FLASHED;
                    if (step < 100)
                        flashes++;
                    flashing = true;
                    for (auto [dr, dc] : DELTAS) {
                        ssize_t nr = static_cast<ssize_t>(r) + dr;
                        ssize_t nc = static_cast<ssize_t>(c) + dc;

                        if (nr < 0 || nc < 0 || nr >= static_cast<ssize_t>(grid.size()) ||
                            nc >= static_cast<ssize_t>(grid[0].size()) || grid[nr][nc] == FLASHED)
                            continue;

                        grid[nr][nc]++;
                    }
                }
            }
        }

        bool all_flashed = true;
        for (auto& row : grid) {
            for (auto& octopus : row) {
                if (octopus == FLASHED)
                    octopus = 0;
                else
                    all_flashed = false;
            }
        }
        if (all_flashed && !all_flash)
            all_flash = step;

        if (all_flash && step >= 100)
            break;
    }

    printf("%zu\n", flashes);
    printf("%zu\n", all_flash + 1);

    return 0;
}
