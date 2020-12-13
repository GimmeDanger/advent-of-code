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


static const vector<pair<int, int>> deltas = {{1, 0}, {-1, 0}, {0, 1}, {0, -1},
                                              {1, 1}, {-1, -1}, {1, -1}, {-1, 1}};

template<typename ScannerT>
int run(vector<string> grid, ScannerT scan, int thr) {
    int rows = grid.size();
    int cols = grid.front().size();
    vector<string> grid_prev = grid;
    int seats_prev = 0;
    int seats = 0;
    while (true) {
        seats = 0;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                auto occupied = scan(grid_prev, r, c);
                char prev_state = grid_prev[r][c];
                char state = prev_state;
                if (prev_state == 'L' && occupied == 0) {
                    state = '#';
                } else if (prev_state == '#' && occupied >= thr) {
                    state = 'L';
                }
                if (state == '#') {
                    seats++;
                }
                grid[r][c] = state;
            }
        }
        if (seats_prev == seats) {
            break;
        }
        swap(seats_prev, seats);
        swap(grid_prev, grid);
    }
    return seats;
}

void solve(istream &is, ostream &os) {
    vector<string> grid;
    for (string line; getline(is, line); ) {
        grid.push_back(move(line));
    }

    int rows = grid.size();
    int cols = grid.front().size();
    auto is_valid = [rows, cols](int r, int c) {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    };

    auto scan_rule_A = [is_valid](vector<string> &grid, int r, int c) {
        int occupied = 0;
        for (auto &d : deltas) {
            int rr = r + d.first;
            int cc = c + d.second;
            if (is_valid(rr, cc) && grid[rr][cc] == '#') {
                occupied++;
            }
        }
        return occupied;
    };
    os << "A: " << run(grid, scan_rule_A, 4) << endl;

    auto scan_rule_B = [is_valid](vector<string> &grid, int r, int c) {
        int occupied = 0;
        for (auto &d : deltas) {
            bool found = false;
            char found_state = '\0';
            for (int mult = 1; mult <= grid.size(); mult++) {
                int rr = r + d.first * mult;
                int cc = c + d.second * mult;
                if (!is_valid(rr, cc)) {
                    break;
                }
                if (grid[rr][cc] != '.') {
                    found = true;
                    found_state = grid[rr][cc];
                    break;
                }
            }
            if (found && found_state == '#') {
                occupied++;
            }
        }
        return occupied;
    };
    os << "B: " << run(grid, scan_rule_B, 5) << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
