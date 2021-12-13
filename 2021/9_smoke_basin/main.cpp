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

const char UNREACHABLE = '9';
const int CC_TO_ANSWER = 3;
const vector<pair<int, int>> adj_deltas = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

void solve_A(const vector<string>& grid) {
    int rows = grid.size();
    int cols = grid.front().size();
    auto is_valid = [rows, cols](int r, int c) {
        return r >= 0 && r < rows && c >= 0 && c < cols;
    };
    auto height = [&grid](int r, int c) {
        return grid[r][c] - '0';
    };
    vector<int> lower_points;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            bool is_lower = true;
            for (auto& d : adj_deltas) {
                int rr = r + d.first;
                int cc = c + d.second;
                if (is_valid(rr, cc) && height(rr, cc) <= height(r, c)) {
                    is_lower = false;
                }
            }
            if (is_lower)
                lower_points.push_back(height(r, c) + 1);
        }
    }
    cout << "A: ";
    cout << accumulate(lower_points.begin(), lower_points.end(), 0) << endl;
}

void dfs(const vector<string>& grid, vector<vector<int>>& cc_visited, unordered_map<int, int>& cc_size, int r, int c, int cnt) {
    int rows = grid.size();
    int cols = grid.front().size();
    auto is_valid = [rows, cols, &grid, &cc_visited](int r, int c) {
        return r >= 0 && r < rows && c >= 0 && c < cols &&
            grid[r][c] != UNREACHABLE && cc_visited[r][c] == 0;
    };
    if (!is_valid(r, c)) {
        return;
    }
    cc_visited[r][c] = cnt;
    cc_size[cnt]++;
    for (auto& d : adj_deltas) {
        int rr = r + d.first;
        int cc = c + d.second;
        if (is_valid(rr, cc)) {
            dfs(grid, cc_visited, cc_size, rr, cc, cnt);
        }
    }
}

void solve_B(const vector<string>& grid) {
    int rows = grid.size();
    int cols = grid.front().size();

    // dfs to get connected component info
    int cnt = 0;
    unordered_map<int, int> cc_size;
    vector<vector<int>> cc_visited(rows, vector<int>(cols, 0));
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (cc_visited[r][c] == 0 && grid[r][c] != UNREACHABLE) {
                dfs(grid, cc_visited, cc_size, r, c, ++cnt);
            }
        }
    }

    // get CC_TO_ANSWER largest components
    assert(CC_TO_ANSWER <= cnt);
    vector<pair<int, int>> r_cc_size;
    r_cc_size.reserve(cc_size.size());
    for (auto [cc, size] : cc_size) {
        r_cc_size.emplace_back(size, cc);
    }
    partial_sort(begin(r_cc_size), begin(r_cc_size) + CC_TO_ANSWER, end(r_cc_size),
                 [](auto &lhs, auto &rhs) { return lhs.first > rhs.first; }
    );

    // return answer
    cout << "B: ";
    cout << accumulate(begin(r_cc_size), begin(r_cc_size) + CC_TO_ANSWER, 1,
                       [](int a, auto &b) { return a * b.first; }) << endl;
}

vector<string> parse(istream& in) {
    string line;
    vector<string> input;
    while(getline(in, line)) {
        input.emplace_back(line);
    }
    return input;
}

int main() {
    ifstream in("input.txt");
    const auto input = parse(in);
    solve_A(input);
    solve_B(input);
    return 0;
}