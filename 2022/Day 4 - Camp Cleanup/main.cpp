#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <unordered_set>
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

pair<int, int> parsePair(string& s) {
    auto it = find(s.begin(), s.end(), '-');
    return {stoi(string(s.begin(), it)), stoi(string(next(it), s.end()))};
}

// https://adventofcode.com/2022/day/4
int solveAImpl(const vector<string> &input) {
    int cnt = 0;
    for (auto& line : input) {
        auto spl = split(line, ',');
        auto lo = parsePair(spl[0]);
        auto hi = parsePair(spl[1]);
        if ((lo.first >= hi.first && lo.second <= hi.second) || (hi.first >= lo.first && hi.second <= lo.second)) {
            cnt++;
        }
    }
    return cnt;
}

int solveBImpl(const vector<string> &input) {
    int cnt = 0;
    for (auto& line : input) {
        auto spl = split(line, ',');
        auto lo = parsePair(spl[0]);
        auto hi = parsePair(spl[1]);
        if (max(lo.first, hi.first) <= min(lo.second, hi.second)) {
            cnt++;
        }
    }
    return cnt;
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