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

vector<string> parse(istream& in) {
    string line;
    vector<string> input;
    while(getline(in, line)) {
        input.emplace_back(line);
    }
    return input;
}

int getScore(char ch) {
    if (ch >= 'a' && ch <= 'z')
        return (ch - 'a' + 1);
    else if (ch >= 'A' && ch <= 'Z')
        return (ch - 'A' + 27);
    else
        assert(0);
}

// https://adventofcode.com/2022/day/3
int solveAImpl(const vector<string> &input) {
    long long score = 0;
    for (auto& line : input) {
        assert(line.size() % 2 == 0);
        // find unique elements in first half
        unordered_set<char> st(line.begin(), line.begin() + line.size() / 2);
        // find first which exists in st
        auto it = find_if(
                line.begin() + line.size() / 2, line.end(),
                [&st](char ch ) { return st.count(ch); });
        assert(it != line.end()); // it exists and it is a single one
        score += getScore(*it);
    }
    return score;
}

int solveBImpl(const vector<string> &input) {
    long long score = 0;
    assert(input.size() % 3 == 0);
    for (int i = 0; i < input.size(); i += 3) {
        // find unique elements in first line
        unordered_set<char> st(input[i].begin(), input[i].end());
        // find overlap with second line
        unordered_set<char> overlap;
        copy_if(input[i + 1].begin(), input[i + 1].end(), 
                std::inserter(overlap, overlap.begin()),
                [&st](char ch ) { return st.count(ch); });
        // find first overlap with the third line
        auto it = find_if(
                input[i + 2].begin(), input[i + 2].end(),
                [&overlap](char ch ) { return overlap.count(ch); });
        assert(it != input[i + 2].end()); // it exists and it is a single one
        score += getScore(*it);
    }
    return score;
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
    const auto input = parse(in);
    solveA(input);
    solveB(input);
    return 0;
}