#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <array>
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

// https://adventofcode.com/2022/day/6
int solve(const vector<string> &input, int window_size = 4) {
    assert(input.size() == 1);
    const auto& seq = input.front();
    unordered_map<char, int> window;
    for (int i = 0; i < seq.size(); i++) {
        if (i < window_size) {
            window[seq[i]]++;
            continue;
        }
        if (window.size() == window_size) {
            return i;
        }
        if (--window[seq[i - window_size]] == 0)
            window.erase(seq[i - window_size]);
        window[seq[i]]++;
    }
    return -1;
}

void solveA(const vector<string>& input) {
    cout << "A: ";
    cout << solve(input,4 /*window_size*/) << endl;
}

void solveB(const vector<string>& input) {
    cout << "B: ";
    cout << solve(input, 14 /*window_size*/) << endl;
}

int main() {
    ifstream in("input.txt");
    const auto input = parseInput(in);
    solveA(input);
    solveB(input);
    return 0;
}