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

constexpr int DAYS = 80;
constexpr int LIFE_COUNTER = 6;
constexpr int INITIAL_LIFE_COUNTER = 8;

long long solve(const vector<string>& input, int days) {
    const auto initial_state = split(input.front(), ',');
    array<long long, INITIAL_LIFE_COUNTER+1> state{};
    for (int s : initial_state) {
        state[s]++;
    }
    for (int d = 1; d <= days; d++) {
        decltype(state) new_state{};
        for (int s = 0; s <= INITIAL_LIFE_COUNTER; s++) {
            if (s == 0 && state[s] > 0) {
                new_state[INITIAL_LIFE_COUNTER] += state[s];
                new_state[LIFE_COUNTER] += state[s];
            }
            else {
                new_state[s - 1] += state[s];
            }
        }
        state = new_state;
//        for (int s = 0; s <= INITIAL_LIFE_COUNTER; s++) {
//            cout << new_state[INITIAL_LIFE_COUNTER - s] << " ";
//        }
//        cout << endl;
//        cout << "after day: " << d << ", sum: " << accumulate(begin(state), end(state), 0ll) << endl;
    }
    return accumulate(begin(state), end(state), 0ll);
}

void solve_A(const vector<string>& input) {
    cout << "A: ";
    cout << solve(input, 80) << endl;
}

void solve_B(const vector<string>& input) {
    cout << "B: ";
    cout << solve(input, 256) << endl;
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