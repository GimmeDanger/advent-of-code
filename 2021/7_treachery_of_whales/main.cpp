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

template<typename Weight>
long long solve(const vector<string>& input, Weight w) {
    const auto positions = split(input.front(), ',');
    auto [min_pos, max_pos] = minmax_element(positions.begin(), positions.end());
    long long min_diff = numeric_limits<long long>::max();
    for (int x = *min_pos; x != *max_pos; x++) {
        long long diff = 0;
        for (int p : positions)
            diff += w(p, x);
        min_diff = min(min_diff, diff);
    }
    return min_diff;
}

void solve_A(const vector<string>& input) {
    cout << "A: ";
    cout << solve(input, [](int k, int x) { return abs(k - x); }) << endl;
}

void solve_B(const vector<string>& input) {
    cout << "B: ";
    cout << solve(input, [](int k, int x) { return abs(k - x) * (abs(k - x) + 1) / 2; }) << endl;
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