#include <bits/stdc++.h>

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
    size_t count = 0;
    size_t group_size = 0;
    unordered_map<char, size_t> mp;
    auto process_group = [&mp, &group_size](size_t& count) {
        if (!mp.empty()) {
            count += count_if(mp.begin(), mp.end(), [group_size](auto& p) {
                return p.second == group_size;
            });
            group_size = 0;
            mp.clear();
        }
    };
    for (string line; getline(is, line);) {
        if (!line.empty()) {
            group_size++;
            for (char ch : line) {
                mp[ch]++;
            }
        } else {
            process_group(count);
        }
    }
    process_group(count);
    os << count << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
