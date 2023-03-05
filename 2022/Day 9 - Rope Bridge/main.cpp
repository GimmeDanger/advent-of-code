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

const vector<pair<int, int>> adj_deltas = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
};

pair<pair<int, int>, int> parseMove(const string& s) {
    pair<int, int> move{};
    switch (s[0]) {
        case 'R':
            move = {0, 1};
            break;
        case 'L':
            move = {0, -1};
            break;
        case 'U':
            move = {-1, 0};
            break;
        case 'D':
            move = {1, 0};
            break;
        default:
            assert(0);
    }
    return {move, stoi(s.substr(2))};
}

// https://adventofcode.com/2022/day/8
int solve(const vector<string>& input, bool verbose = false) {
    unordered_map<int, unordered_set<int>> visited;
    pair<int, int> posHead{}, posTail{};
    visited[posTail.first].insert(posTail.second);
    for (auto& line : input) {
        auto [move, cnt] = parseMove(line);
        while (cnt--) {
            auto prevPosHead = posHead;
            posHead.first += move.first;
            posHead.second += move.second;
            if (abs(posHead.first - posTail.first) > 1 || abs(posHead.second - posTail.second) > 1) {
                auto posDiff = abs(posHead.first - posTail.first) + abs(posHead.second - posTail.second);
                if (posDiff == 2) {
                    posTail.first += move.first;
                    posTail.second += move.second;
                } else if (posDiff > 2) {
                    posTail = prevPosHead;
                }
                visited[posTail.first].insert(posTail.second);
            }
        }
    }
    int res = 0;
    for (auto& [k, v] : visited) {
        res += v.size();
    }
    return res;
}

void solveA(const vector<string>& input) {
    cout << "A: ";
    cout << solve(input) << endl;
}

void solveB(const vector<string>& input) {
    cout << "B: ";
}

int main() {
    ifstream in("input.txt");
    const auto input = parseInput(in);
    solveA(input);
    solveB(input);
    return 0;
}