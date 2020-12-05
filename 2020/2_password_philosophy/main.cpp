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

// Valid Password = if # of ch occurences in range [lo, hi]
size_t solve_A(istream &is) {
    int lo, hi;
    char dummy, ch;
    size_t valid_pwd_cnt = 0;
    for (string line; getline(is, line);) {
        stringstream ss(line);
        ss >> lo >> dummy >> hi >> ch >> dummy >> line;
        auto count = count_if(begin(line), end(line), [ch](auto curr) { return curr == ch; });
        if (count >= lo && count <= hi) {
            valid_pwd_cnt++;
        }
    }
    return valid_pwd_cnt;
}

// Valid Password = if (pwd[lo+1] == ch) ^ (pwd[hi+1] == ch)
// (only one position contains)
size_t solve_B(istream &is) {
    int lo, hi;
    char dummy, ch;
    size_t valid_pwd_cnt = 0;
    for (string line; getline(is, line);) {
        stringstream ss(line);
        ss >> lo >> dummy >> hi >> ch >> dummy >> line;
        lo--; hi--;
        if ((lo < line.size() && line[lo] == ch) ^
            (hi < line.size() && line[hi] == ch)) {
            valid_pwd_cnt++;
        }
    }
    return valid_pwd_cnt;
}

void solve(istream &is, ostream &os) {

    if (0) {
        os << "A answer = " << solve_A(is) << endl;
    }
    else {
        os << "A answer = " << solve_B(is) << endl;
    }
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
