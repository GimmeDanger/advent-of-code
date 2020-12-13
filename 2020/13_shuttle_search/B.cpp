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

long long euclide_reverse(long long a, long long n) {
    long long t = 0;
    long long r = n;
    long long newt = 1;
    long long newr = a;
    while (newr != 0) {
        long long q = r / newr;
        {
            long long tmp = newt;
            newt = t - q * newt;
            t = tmp;
        }
        {
            long long tmp = newr;
            newr = r - q * newr;
            r = tmp;
        }
    }
    if (r > 1) {
        throw invalid_argument("a is not invertible");
    }
    if (t < 0) {
        t = t + n;
    }
    return t;
}

void solve(istream &is, ostream &os) {
    int r = 0, dummy = 0;
    is >> dummy;
    long long CRT_mult = 1;
    vector<pair<int, int>> equations;
    for (string entry; getline(is, entry, ','); r++) {
        if (entry.empty() || entry.front() == 'x') {
            continue;
        }
        int ts = stoi(entry);
        equations.emplace_back(-r, ts);
        CRT_mult *= ts;
    }
    long long x = 0;
    for (auto& [r_i, a_i] : equations) {
        long long m_i = CRT_mult / a_i;
        long long r_m_i = euclide_reverse(m_i, a_i);
        x += (r_i * m_i * r_m_i) % CRT_mult;
        x = (x + CRT_mult) % CRT_mult;
        // x += (((r_i * m_i + CRT_mult) % CRT_mult) * r_m_i + CRT_mult) % CRT_mult;
    }
    os << x << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
