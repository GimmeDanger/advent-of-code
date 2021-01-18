#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <optional>
#include <array>
#include <cctype>
#include <cstring>
#include <string>
#include <string_view>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <charconv>
#include <algorithm>
#include <functional>
#include <numeric>

using namespace std;

#define dbg 1

#if dbg
#define INPUT_SOURCE() ifstream cin("input.txt")
#else
#define INPUT_SOURCE()
#endif

#define FAST_IO()              \
  ios::sync_with_stdio(false); \
  cout.tie(nullptr);           \
  cin.tie(nullptr)

void solve(istream &is, ostream &os) {
    unordered_map<long long, long long> mem;

    auto set_bit = [](long long &address, int pos) {
        address |= (1ll << pos);
    };

    auto unset_bit = [](long long &address, int pos) {
        if (address & (1ll << pos))
            address ^= (1ll << pos);
    };

    string bit_mask;
    auto apply_mask = [&os, &bit_mask, set_bit, unset_bit](const long long address) -> vector<long long> {
        size_t x_count = count(begin(bit_mask), end(bit_mask), 'X');
        int variants = 1 << x_count;
        vector<long long> res(variants);
        for (int b = 0; b < variants; b++) {
            long long virtual_address = address;
            for (int pos = 0, x_pos = 0; pos < bit_mask.size(); pos++) {
                if (bit_mask[pos] == '1') {
                    set_bit(virtual_address, pos);
                } else if (bit_mask[pos] == 'X') {
                    if (b & (1ll << x_pos)) {
                        set_bit(virtual_address, pos);
                    } else {
                        unset_bit(virtual_address, pos);
                    }
                    x_pos++;
                }
            }
            res[b] = virtual_address;
        }
        os << res.size() << endl;
        return res;
    };

    for (string lhs; is >> lhs; ) {
        char dummy;
        long long val;
        if (lhs == "mask") {
            is >> dummy >> bit_mask;
            reverse(bit_mask.begin(), bit_mask.end());
        } else {
            lhs.pop_back();
            int pos = stoi(lhs.substr(4));
            is >> dummy >> val;
            for (auto virtual_pos : apply_mask(pos)) {
                mem[virtual_pos] = val;
            }
        }
    }

    // What is the sum of all values left in memory after it completes?
    long long sum = accumulate(begin(mem), end(mem), 0ll,
                               [](long long value, const auto &p) { return value + p.second; }
    );
    os << "B: " << sum << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
