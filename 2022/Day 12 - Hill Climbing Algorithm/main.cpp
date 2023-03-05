#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <queue>
#include <unordered_set>
#include <unordered_map>
using namespace std;

vector<string> split(string s, char delimiter) {
    size_t pos_start = 0, pos_end;
    string token;
    vector<string> res;
    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + 1;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}

vector<string> parseInput(istream& in) {
    string line;
    vector<string> input;
    while(getline(in, line)) {
        input.emplace_back(line);
    }
    return input;
}

const vector<pair<int, int>> adj_deltas = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
};

int bfs(const vector<string>& grid, unordered_set<char> SRC, char DST = 'E') {
    int rows = grid.size();
    int cols = grid[0].size();

    vector<pair<int, int>> multi_src;
    int r_dst = -1, c_dst = -1;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (SRC.count(grid[r][c])) {
                multi_src.push_back({r, c});
            }
            else if (grid[r][c] == DST) {
                r_dst = r;
                c_dst = c;
            }
        }
    }

    vector<vector<int>> dist(rows, vector<int>(cols, -1));
    auto is_valid = [&](int r, int c) {
        return r >= 0 && r < rows && c >= 0 && c < cols && dist[r][c] < 0;
    };
    auto pass_condition = [&](int r, int c, int rr, int cc) {
        int h_src = (SRC.count(grid[r][c]) ? 0 : grid[r][c] == DST ? 'z' - 'a' : grid[r][c] - 'a');
        int h_dst = (SRC.count(grid[rr][cc]) ? 0 : grid[rr][cc] == DST ? 'z' - 'a' : grid[rr][cc] - 'a');
        return h_dst - h_src <= 1;
    };
    queue<pair<int, int>> q;
    // bfs init
    for (auto& p : multi_src) {
        q.push({p.first, p.second});
        dist[p.first][p.second] = 0;
    }
    // bfs loop
    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        if (grid[r][c] == DST) {
            return dist[r][c];
        }
        for (auto [r_d, c_d] : adj_deltas) {
            int rr = r + r_d, cc = c + c_d;
            if (is_valid(rr, cc) && pass_condition(r, c, rr, cc)) {
                dist[rr][cc] = dist[r][c] + 1;
                q.push({rr, cc});
            }
        }
    }
    return -1;
}

void solveA(const vector<string>& input) {
    cout << "A: " << endl;
    cout << bfs(input, {'S'}) << endl;
}

void solveB(const vector<string>& input) {
    cout << "B: " << endl;
    cout << bfs(input, {'S', 'a'}) << endl;
}

int main() {
    ifstream in("input.txt");
    const auto input = parseInput(in);
    solveA(input);
    solveB(input);
    return 0;
}