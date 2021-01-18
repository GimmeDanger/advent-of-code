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

vector<tuple<int, int, int, int>> compute_deltas() {
    array<int, 3> d = {-1, 0, 1};
    vector<tuple<int, int, int, int>> deltas;
    for (int x : d) {
        for (int y : d) {
            for (int z : d) {
                for (int w : d) {
                    if (x == 0 && y == 0 && z == 0 && w == 0) {
                        continue;
                    }
                    deltas.push_back({x, y, z, w});
                }
            }
        }
    }
    assert(deltas.size() == 80);
    return deltas;
};

void solve(istream &is, ostream &os) {
    constexpr int dim = 30;
    constexpr int cycles = 6;
    constexpr int w_0 = dim / 2, z_0 = dim / 2, y_0 = dim / 2, x_0 = dim / 2;
    const auto deltas = compute_deltas();
    vector<vector<vector<vector<char>>>> grid(dim, vector<vector<vector<char>>>(dim, vector<vector<char>>(dim, vector<char>(dim, '.'))));
    {
        int x = x_0;
        for (string line; getline(is, line); ) {
            for (int i = 0; i < line.size(); i++) {
                if (line[i] == '#') {
                    grid[x][y_0 + i][z_0][w_0] = '#';
                }
            }
            x++;
        }
    }

    auto valid_coord = [dim](int x) {
        return x >= 0 && x < dim;
    };

    auto scan = [&grid, &deltas, valid_coord](int x, int y, int z, int w) {
        int active_cnt = 0;
        for (auto& t : deltas) {
            int xx = x + get<0>(t);
            int yy = y + get<1>(t);
            int zz = z + get<2>(t);
            int ww = w + get<3>(t);
            if (valid_coord(xx) && valid_coord(yy) && valid_coord(zz) && valid_coord(ww)) {
                if (grid[xx][yy][zz][ww] == '#') {
                    active_cnt++;
                }
            }
        }
        return active_cnt;
    };

    vector<tuple<int, int, int, int>> need_swap;
    for (int c = 0; c < cycles; c++) {
        need_swap.clear();
        for (int x = 0; x < dim; x++) {
            for (int y = 0; y < dim; y++) {
                for (int z = 0; z < dim; z++) {
                    for (int w = 0; w < dim; w++) {
                        int active_cnt = scan(x, y, z, w);
                        if ((grid[x][y][z][w] == '#' && (active_cnt < 2 || active_cnt > 3)) ||
                            (grid[x][y][z][w] == '.' && (active_cnt == 3))) {
                            need_swap.push_back({x, y, z, w});
                        }
                    }
                }
            }
        }
        for (auto& t : need_swap) {
            int x = get<0>(t);
            int y = get<1>(t);
            int z = get<2>(t);
            int w = get<3>(t);
            if (grid[x][y][z][w] == '#') {
                grid[x][y][z][w] = '.';
            } else {
                grid[x][y][z][w] = '#';
            }
        }
    }

    int result = 0;
    for (int x = 0; x < dim; x++) {
        for (int y = 0; y < dim; y++) {
            for (int z = 0; z < dim; z++) {
                for (int w = 0; w < dim; w++) {
                    if (grid[x][y][z][w] == '#') {
                        result++;
                    }
                }
            }
        }
    }
    os << "B: " << result << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
