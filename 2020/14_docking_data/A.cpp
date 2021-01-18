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
    unordered_map<int, long long> mem;

    string bit_mask;
    auto apply_mask = [&bit_mask](long long val) {
        for (int pos = 0; pos < bit_mask.size(); pos++) {
            if (bit_mask[pos] == '0') {
                if (val & (1ll << pos))
                    val ^= (1ll << pos);
            }
            else if (bit_mask[pos] == '1') {
                val |= (1ll << pos);
            }
        }
        return val;
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
            mem[pos] = apply_mask(val);
        }
    }

    // What is the sum of all values left in memory after it completes?
    long long sum = accumulate(begin(mem), end(mem), 0ll,
                               [](long long value, const auto &p) { return value + p.second; }
    );
    os << "A: " << sum << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
