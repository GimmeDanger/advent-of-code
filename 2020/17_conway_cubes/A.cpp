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

static vector<tuple<int, int, int>> deltas = {
        {1, 0, 0}, {-1, 0, 0},
        {0, 1, 0}, {0, -1, 0},
        {1, 1, 0}, {-1, -1, 0},
        {-1, 1, 0}, {1, -1, 0},
        {0, 0, 1}, {1, 0, 1}, {-1, 0, 1},
        {0, 1, 1}, {0, -1, 1},
        {1, 1, 1}, {-1, -1, 1},
        {-1, 1, 1}, {1, -1, 1},
        {0, 0, -1}, {1, 0, -1}, {-1, 0, -1},
        {0, 1, -1}, {0, -1, -1},
        {1, 1, -1}, {-1, -1, -1},
        {-1, 1, -1}, {1, -1, -1}
};

void solve(istream &is, ostream &os) {
    assert(deltas.size() == 26);
    constexpr int dim = 40;
    constexpr int cycles = 6;
    constexpr int z_0 = dim / 2, y_0 = dim / 2, x_0 = dim / 2;
    vector<vector<vector<char>>> grid(dim, vector<vector<char>>(dim, vector<char>(dim, '.')));
    {
        int x = x_0;
        for (string line; getline(is, line); ) {
            assert(line.size() < dim / 4);
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == '#') {
                    grid[x][y_0 + i][z_0] = '#';
                }
            }
            x++;
        }
    }

    auto valid_coord = [dim](int x) {
        return x >= 0 && x < dim;
    };

    auto scan = [&grid, valid_coord](int x, int y, int z) {
        int active_cnt = 0;
        for (auto& t : deltas) {
            int xx = x + get<0>(t);
            int yy = y + get<1>(t);
            int zz = z + get<2>(t);
            if (valid_coord(xx) && valid_coord(yy) && valid_coord(zz)) {
                if (grid[xx][yy][zz] == '#') {
                    active_cnt++;
                }
            }
        }
        return active_cnt;
    };

    vector<tuple<int, int, int>> need_swap;
    for (int c = 0; c < cycles; c++) {
        need_swap.clear();
        for (int x = 0; x < dim; x++) {
            for (int y = 0; y < dim; y++) {
                for (int z = 0; z < dim; z++) {
                    int active_cnt = scan(x, y, z);
                    if ((grid[x][y][z] == '#' && (active_cnt < 2 || active_cnt > 3)) ||
                        (grid[x][y][z] == '.' && (active_cnt == 3))) {
                        need_swap.push_back({x, y, z});
                    }
                }
            }
        }
        for (auto& t : need_swap) {
            int x = get<0>(t);
            int y = get<1>(t);
            int z = get<2>(t);
            if (grid[x][y][z] == '#') {
                grid[x][y][z] = '.';
            } else {
                grid[x][y][z] = '#';
            }
        }
    }

    int result = 0;
    for (int x = 0; x < dim; x++) {
        for (int y = 0; y < dim; y++) {
            for (int z = 0; z < dim; z++) {
                if (grid[x][y][z] == '#') {
                    result++;
                }
            }
        }
    }
    os << "A: " << result << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
