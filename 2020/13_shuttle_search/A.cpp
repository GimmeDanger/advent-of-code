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
    int arrive_time = 0;
    is >> arrive_time;
    pair<int, int> res = {numeric_limits<int>::max(), 0};
    for (string entry; getline(is, entry, ','); ) {
        if (entry.empty() || entry.front() == 'x') {
            continue;
        }
        int ts = stoi(entry);
        int wt = ts - arrive_time % ts;
        if (wt < res.first) {
            res.first = wt;
            res.second = ts;
        }
    }
    os << res.first * res.second << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
