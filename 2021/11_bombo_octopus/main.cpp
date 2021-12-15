#include <iostream>
#include <utility>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <deque>
#include <queue>
#include <map>
#include <cassert>
#include <random>
#include <algorithm>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <iterator>
using namespace std;

vector<int> split(string s, char delimiter) {
    size_t pos_start = 0, pos_end;
    string token;
    vector<int> res;
    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + 1;
        res.push_back(stoi(token));
    }
    res.push_back(stoi(s.substr(pos_start)));
    return res;
}

vector<string> parse(istream& in) {
    string line;
    vector<string> input;
    while(getline(in, line)) {
        input.emplace_back(line);
    }
    return input;
}

constexpr char READY_TO_FLASH = '@';
constexpr char RESTING = '#';
const vector<pair<int, int>> adj_deltas = {
        {1, 0}, {-1, 0},
        {-1, 1}, {0, 1}, {1, 1},
        {-1, -1}, {0, -1}, {1, -1}
};

void increase(char& v) {
    int d = (v != RESTING) ? v - '0' : 0;
    if (d < 9)
        v = char(d + 1) + '0';
    else
        v = READY_TO_FLASH;
}

void solve(const vector<string> &input, unsigned steps, unsigned& flashes, unsigned& first_sync_step) {
    // ret
    flashes = 0;
    first_sync_step = 0;

    vector<string> grid = input;
    deque<pair<int, int>> ready_to_flash;
    int rows = grid.size();
    int cols = grid.front().size();
    auto is_valid = [rows, cols, &grid](int r, int c) {
      return r >= 0 && r < rows && c >= 0 && c < cols &&
             grid[r][c] != READY_TO_FLASH && grid[r][c] != RESTING;
    };

    for (int s = 1; s <= steps; s++) {
        // increase energy
        unsigned resting_at_start_of_step = 0;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (grid[r][c] == RESTING)
                    resting_at_start_of_step++;
                increase(grid[r][c]);
                if (grid[r][c] == READY_TO_FLASH) {
                    ready_to_flash.emplace_back(r, c);
                }
            }
        }
        if (resting_at_start_of_step == rows * cols && first_sync_step == 0) {
            first_sync_step = s - 1;
        }
        // run flashes
        while (!ready_to_flash.empty()) {
            auto [r, c] = ready_to_flash.front();
            ready_to_flash.pop_front();
            grid[r][c] = RESTING;
            flashes++;
            for (auto& d : adj_deltas) {
                int rr = r + d.first;
                int cc = c + d.second;
                if (is_valid(rr, cc)) {
                    increase(grid[rr][cc]);
                    if (grid[rr][cc] == READY_TO_FLASH) {
                        ready_to_flash.emplace_back(rr, cc);
                    }
                }
            }
        }
    }
}

void solve_A(const vector<string> &input) {
    unsigned flashes = 0;
    unsigned first_sync_step = 0;
    solve(input, 100, flashes, first_sync_step);
    cout << "A: ";
    cout << flashes << endl;
}

void solve_B(const vector<string>& grid) {
    unsigned flashes = 0;
    unsigned first_sync_step = 0;
    solve(grid, 400, flashes, first_sync_step);
    cout << "B: ";
    cout << first_sync_step << endl;
}

int main() {
    ifstream in("input.txt");
    const auto input = parse(in);
    solve_A(input);
    solve_B(input);
    return 0;
}