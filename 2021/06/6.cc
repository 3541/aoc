#include <fstream>
#include <vector>
#include <cstdio>

using namespace std;

size_t simulate(vector<uint8_t>& fish_in, size_t days) {
    size_t fish[9] = { 0 };
    for (auto const& f : fish_in)
        fish[f]++;

    for (size_t i = 0; i < days; i++) {
        size_t new_0 = fish[1];
        for (size_t f = 1; f < 8; f++)
            fish[f] = fish[f + 1];
        fish[8] = fish[0];
        fish[6] += fish[0];
        fish[0] = new_0;
    }

    size_t sum = 0;
    for (size_t i = 0; i < 9; i++)
        sum += fish[i];

    return sum;
}

int main() {
    ifstream in { "data" };

    vector<uint8_t> fish;
    char c;
    while (in >> c) {
        if ('0' <= c && c <= '9')
            fish.push_back(c - '0');
    }

    printf("%zu\n", simulate(fish, 80));
    printf("%zu\n", simulate(fish, 256));

    return 0;
}
