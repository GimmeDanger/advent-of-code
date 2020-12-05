#include <iostream>
#include <vector>
#include <fstream>
#include <optional>
#include <array>

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
    // get grid of the forrest
    vector<vector<char>> grid;
    for (string line; getline(is, line); ) {
        grid.emplace_back(line.begin(), line.end());
    }

    // solver: count trees for specific slope params
    auto count_trees = [&grid](int row_delta, int col_delta) {
        const int rows = grid.size(), cols = grid[0].size();
        auto is_tree = [](char ch) {
            return ch == '#';
        };
        size_t trees_cnt = 0;
        for (int r = 0, c = 0; r < rows; r += row_delta, c = (c + col_delta) % cols) {
            if (is_tree(grid[r][c])) {
                trees_cnt++;
            }
        }
        return trees_cnt;
    };

    // compute weird output
    long long ret = 1;
    for (auto &delta : vector<pair<int, int>>{{1, 1},
                                              {1, 3},
                                              {1, 5},
                                              {1, 7},
                                              {2, 1}}) {
        ret *= count_trees(delta.first, delta.second);
    }
    os << ret << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
