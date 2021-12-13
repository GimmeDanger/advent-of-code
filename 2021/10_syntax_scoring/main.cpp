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
#include <iterator>
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

vector<string> parse(istream& in) {
    string line;
    vector<string> input;
    while(getline(in, line)) {
        input.emplace_back(line);
    }
    return input;
}

const unordered_map<char, char> clossing = {
        {')', '('}, {']', '['}, {'}', '{'}, {'>', '<'}
};
const unordered_map<char, char> openning = {
        {'(', ')'}, {'[', ']'}, {'{', '}'}, {'<', '>'}
};

size_t validate(const string& line, vector<char>& st) {
    st.clear();
    for (size_t i = 0; i < line.size(); i++) {
        char ch = line[i];
        auto it = clossing.find(ch);
        if (it == end(clossing))
            st.push_back(ch);
        else if (!st.empty() && st.back() == it->second)
            st.pop_back();
        else
            return i;
    }
    return line.size();
}

void solve_A(const vector<string> &input) {
    const unordered_map<char, int> scores = {
            {')', 3},
            {']', 57},
            {'}', 1197},
            {'>', 25137}};
    long long points = 0;
    vector<char> st;
    for (const auto &line : input) {
        size_t pos = validate(line, st);
        if (pos < line.size())
            points += scores.find(line[pos])->second;
    }
    cout << "A: ";
    cout << points << endl;
}

void solve_B(const vector<string>& input) {
    const unordered_map<char, int> scores = {
            {')', 1},
            {']', 2},
            {'}', 3},
            {'>', 4}};
    vector<long long> line_scores;
    vector<char> st;
    for (const auto &line : input) {
        size_t pos = validate(line, st);
        if (pos < line.size())
            continue;
        long long local_score = 0;
        while (!st.empty()) {
            local_score *= 5;
            local_score += scores.find(openning.find(st.back())->second)->second;
            st.pop_back();
        }
        line_scores.push_back(local_score);
    }
    cout << "B: ";
    nth_element(begin(line_scores),
                begin(line_scores) + line_scores.size() / 2,
                end(line_scores));
    cout << line_scores[line_scores.size() / 2] << endl;
}

int main() {
    ifstream in("input.txt");
    const auto input = parse(in);
    solve_A(input);
    solve_B(input);
    return 0;
}