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

struct state {
    int x = 0;
    int y = 0;
    char dir = 'E';
    int dir_id = 0;

    static constexpr array<char, 4> dirs = {'E', 'S', 'W', 'N'};

    void action(char com, int val) {
        if (com == 'N' || com == 'S') {
            y += (com == 'S' ? -1 : 1) * val;
        }
        else if (com == 'E' || com == 'W') {
            x += (com == 'W' ? -1 : 1) * val;
        }
        else if (com == 'L' || com == 'R') {
            rotate((com == 'L' ? -1 : 1) * val);
        }
        else if (com == 'F') {
            forward(val);
        }
    }

    void forward(int val) {
        action(dir, val);
    }

    void rotate(int deg) {
        int turns = deg / 90;
        dir_id = (dir_id + turns + 4) % 4;
        dir = dirs[dir_id];
    }
};

int dist(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

void solve(istream &is, ostream &os) {
    state st;
    char com; int val;
    while (is >> com >> val) {
        st.action(com, val);
    }
    os << "A: " << dist(st.x, st.y, 0, 0) << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
