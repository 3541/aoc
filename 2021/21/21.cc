#include <cstdint>
#include <cstdio>
#include <unordered_map>
#include <utility>

using namespace std;

using Player = pair<uint8_t, uint8_t>;
using State  = tuple<Player, Player, bool>;

namespace std {

template <>
struct hash<State> {
    ssize_t operator()(State const& s) const {
        auto [p1, p2, p1_turn] = s;
        return hash<uint8_t> {}(p1.first) ^ hash<uint8_t> {}(p1.second) ^
               hash<uint8_t> {}(p2.first) ^ hash<uint8_t> {}(p2.second) ^ hash<bool> {}(p1_turn);
    }
};

pair<size_t, size_t>& operator+=(pair<size_t, size_t>& lhs, pair<size_t, size_t> const& rhs) {
    lhs.first += rhs.first;
    lhs.second += rhs.second;

    return lhs;
}

pair<size_t, size_t> wins(State state) {
    static unordered_map<State, pair<size_t, size_t>> memo;
    auto                                              res = memo.find(state);
    if (res != memo.end())
        return res->second;

    auto [p1, p2, p1_turn] = state;
    auto ret               = make_pair<size_t, size_t>(0, 0);
    for (uint8_t i = 1; i <= 3; i++) {
        for (uint8_t j = 1; j <= 3; j++) {
            for (uint8_t k = 1; k <= 3; k++) {
                size_t roll = i + j + k;
                State  next_state;
                get<2>(next_state) = !p1_turn;
                if (p1_turn) {
                    auto next_pos   = (p1.first + roll - 1) % 10 + 1;
                    auto next_score = p1.second + next_pos;
                    if (next_score >= 21) {
                        ret.first++;
                        continue;
                    }
                    get<0>(next_state) = make_pair(next_pos, next_score);
                    get<1>(next_state) = p2;
                } else {
                    auto next_pos   = (p2.first + roll - 1) % 10 + 1;
                    auto next_score = p2.second + next_pos;
                    if (next_score >= 21) {
                        ret.second++;
                        continue;
                    }
                    get<0>(next_state) = p1;
                    get<1>(next_state) = make_pair(next_pos, next_score);
                }

                ret += wins(next_state);
            }
        }
    }

    memo.insert(make_pair(state, ret));
    return ret;
}

} // namespace std

int main(void) {
    auto [p1wins, p2wins] = wins({ { 8, 0 }, { 4, 0 }, true });

    printf("%zu\n", max(p1wins, p2wins));
}
