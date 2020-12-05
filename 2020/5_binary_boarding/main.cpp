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

template<char L, char R>
int bin_decoder(string_view s) {
    int lo = 0;
    int hi = (1 << s.size()) - 1;
    for (char ch : s) {
        int mi = lo + (hi - lo) / 2;
        if (ch == L) {
            hi = mi - 1;
        } else if (ch == R) {
            lo = mi + 1;
        } else {
            throw std::invalid_argument("");
        }
    }
    return lo;
}

int index(int r, int c) {
    return r * 8 + c;
}

void solve(istream &is, ostream &os) {
    assert((bin_decoder<'F', 'B'>(string("FBFBBFF")) == 44));
    assert((bin_decoder<'F', 'B'>(string("FFFBBBF")) == 14));
    assert((bin_decoder<'F', 'B'>(string("BBFFBBF")) == 102));

    if (0) {
        int res = 0;
        for (string s; is >> s; ) {
            int row = bin_decoder<'F', 'B'>(s.substr(0, 7));
            int col = bin_decoder<'L', 'R'>(s.substr(7));
            res = max(res, index(row, col));
        }
        os << res << endl;
    }
    else {
        const int rows = 128, cols = 8;
        vector<vector<bool>> seats(rows, vector<bool>(cols, false));
        for (string s; is >> s; ) {
            int row = bin_decoder<'F', 'B'>(s.substr(0, 7));
            int col = bin_decoder<'L', 'R'>(s.substr(7));
            seats[row][col] = true;
        }

        // visualize to find seat coords
        for (int r = 0; r < rows; r++) {
            cout << r << endl;
            for (int c = 0; c < cols; c++) {
                cout << (seats[r][c] ? "*" : "0");
            }
            cout << endl;
        }
        os << endl;

        os << "ID: " << index(77, 3) << endl;
    }
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
