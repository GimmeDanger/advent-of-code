#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <map>
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

// https://adventofcode.com/2022/day/8
vector<vector<int>> parseGrid(const vector<string> &input, bool verbose = false) {
    cout << endl;
    vector<vector<int>> grid;
    for (const auto& line : input) {
        grid.push_back(vector<int>(line.size(), 0));
        for (int i = 0; i < line.size(); i++) {
            grid.back()[i] = (line[i] - '0');
        }
    }
    if (verbose) {
        for (auto& row : grid) {
            for (int n : row) {
                cout << n;
            }
            cout << endl;
        }
    }
    return grid;
}

const vector<pair<int, int>> adj_deltas = {
        {+1, 0}, {-1, 0}, {0, +1}, {0, -1}
};

int solveAImpl(const vector<string>& input, bool verbose = false) {
    auto grid = parseGrid(input, verbose);
    int visibleCnt = 0, topScore = 0;
    int rows = grid.size();
    int cols = grid[0].size();
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int score = 1;
            for (auto& d : adj_deltas) {
                bool hidden = false;
                for (int cc = c + d.second, rr = r + d.first;
                     cc >= 0 && cc < cols && rr >= 0 && rr < rows && !hidden;
                     cc += d.second, rr += d.first) {
                    if (grid[r][c] <= grid[rr][cc]) {
                        hidden = true;
                    }
                }
                if (!hidden) {
                    visibleCnt++;
                    break;
                }
            }
        }
    }
    return visibleCnt;
}

int solveBImpl(const vector<string>& input, bool verbose = false) {
    auto grid = parseGrid(input, verbose);
    int topScore = 0;
    int rows = grid.size();
    int cols = grid[0].size();
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int score = 1;
            for (auto& d : adj_deltas) {
                bool hidden = false;
                int cc = c + d.second, rr = r + d.first;
                for (; cc >= 0 && cc < cols && rr >= 0 && rr < rows; cc += d.second, rr += d.first) {
                    if (grid[r][c] <= grid[rr][cc]) {
                        break;
                    }
                }
                score *= (max(abs(rr - r), abs(cc - c)) - (!(cc >= 0 && cc < cols && rr >= 0 && rr < rows) ? 1 : 0));
            }
            topScore = max(score, topScore);
        }
    }
    return topScore;
}

void solveA(const vector<string>& input) {
    cout << "A: ";
    cout << solveAImpl(input) << endl;
}

void solveB(const vector<string>& input) {
    cout << "B: ";
    cout << solveBImpl(input) << endl;
}

int main() {
    ifstream in("input.txt");
    const auto input = parseInput(in);
    solveA(input);
    solveB(input);
    return 0;
}