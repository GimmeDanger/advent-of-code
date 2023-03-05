#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
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

vector<string> parse(istream& in) {
    string line;
    vector<string> input;
    while(getline(in, line)) {
        input.emplace_back(line);
    }
    return input;
}

// https://adventofcode.com/2022/day/1
int solve(const vector<string> &input, int top_k = 1) {
    vector<long long> elfCalories;
    for (long long curr = 0, i = 0; i < input.size(); i++) {
        curr += !input[i].empty() ? stoi(input[i]) : 0;
        if (i + 1 == input.size() || input[i].empty()) {
            elfCalories.push_back(curr);
            curr = 0;
        }
    }
    // O(top_k * log (top_k)) in time
    partial_sort(elfCalories.begin(), elfCalories.begin() + top_k, elfCalories.end(), greater<>());
    // O(top_k) in time
    return accumulate(elfCalories.begin(), elfCalories.begin() + top_k, 0ll);
}

void solve_A(const vector<string>& input) {
    cout << "A: ";
    cout << solve(input, 1 /*top_k*/) << endl;
}

void solve_B(const vector<string>& input) {
    cout << "B: ";
    cout << solve(input, 3 /*top_k*/) << endl;
}

int main() {
    ifstream in("input.txt");
    const auto input = parse(in);
    solve_A(input);
    solve_B(input);
    return 0;
}