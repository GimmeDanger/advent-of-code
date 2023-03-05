#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <array>
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

pair<string, optional<int>> parseInstr(const string& s) {
    auto spl = split(s, ' ');
    if (spl.size() == 1) {
        return {spl[0], nullopt};
    }
    else if (spl.size() == 2) {
        return {spl[0], stoi(spl[1])};
    }
    else {
        assert(0);
        return {"", {}};
    }
}

// https://adventofcode.com/2022/day/10
unordered_map<int, int> solve(const vector<string>& input, bool verbose = false) {
    unordered_map<int, int> cycleStats;
    int cycles = 1, x = 1;
    for (const auto& line : input) {
        cycleStats[cycles] = x;
        auto [cmd, val] = parseInstr(line);
        if (cmd == "noop") {
            cycles++;
        }
        else if (cmd == "addx") {
            cycles++;
            cycleStats[cycles] = x;
            x += val.value();
            cycles++;
        }
    }
    cycleStats[cycles] = x;
    return cycleStats;
}

void solveA(const vector<string>& input) {
    cout << "A: " << endl;
    auto stats = solve(input);
    int res = 0;
    for (int c = 20; c <= stats.size(); c += 40) {
        res += stats[c] * c;
    }
    cout << res << endl;
}

void solveB(const vector<string>& input) {
    cout << "B: " << endl;
    auto stats = solve(input);
    array<array<char, 40>, 6> grid{};
    for (int r = 0; r < grid.size(); r++) {
        for (int c = 0; c < grid[0].size(); c++) {
            int cycle = (r * grid[0].size() + c + 1);
            int x = stats[cycle];
            char ch = (c >= x - 1 && c <= x + 1) ? '#' : '.';
            cout << ch;
        }
        cout << endl;
    };
}

int main() {
    ifstream in("input.txt");
    const auto input = parseInput(in);
    solveA(input);
    solveB(input);
    return 0;
}