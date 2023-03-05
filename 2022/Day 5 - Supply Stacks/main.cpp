#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <array>
using namespace std;

vector<string> parseInput(istream& in) {
    string line;
    vector<string> input;
    while(getline(in, line)) {
        input.emplace_back(line);
    }
    return input;
}

char getLabel(const string& s, int pos, int stackNum) {
    // label[i] = 3 * pos + 2
    // example: [X] [Y] ... [Z]
    // label[0] = s[3 * 0 + 2] = X
    // label[1] = s[3 * 1 + 2] = s[5] = Y
    // and so on
    assert(pos < stackNum);
    int digitPos = 4 * pos + 1;
    if (digitPos < s.size())
        return s[digitPos];
    return ' ';
}

// crates are moved one by one (if multipleBatch=false), so we need to reverse the tail of dest
// otherwise it doesn't needed
void moveCrates(vector<int>& src, vector<int>& dst, int cnt, bool multipleBatch) {
    copy(src.begin() + (src.size() - cnt), src.end(), back_inserter(dst));
    src.resize(src.size() - cnt);
    if (!multipleBatch) {
        reverse(dst.begin() + (dst.size() - cnt), dst.end());
    }
}

// https://adventofcode.com/2022/day/5
string solve(const vector<string> &input, int stackNum, bool multipleBatch) {
    // parse initial state
    int i = 0;
    vector<vector<int>> stacks(stackNum);
    for (; i < input.size(); i++) {
        auto& line = input[i];
        if (isdigit(getLabel(line, 0, stackNum)))
            break;
        for (int j = 0; j < stackNum; j++) {
            char ch = getLabel(line, j, stackNum);
            if (ch != ' ') {
                stacks[j].push_back(ch);
            }
        }
    }
    for (int j = 0; j < stackNum; j++) {
        reverse(stacks[j].begin(), stacks[j].end());
    }
    i += 2;
    // parse and perform operations
    for (; i < input.size(); i++) {
        auto& line = input[i];
        // parse
        array<int, 3> parsedOperation{};
        auto it = line.begin();
        for (int j = 0; j < 3; j++) {
            auto numStart = find_if(it, line.end(), [](char ch) { return isdigit(ch); });
            auto numEnd = find_if(numStart, line.end(), [](char ch) { return !isdigit(ch); });
            parsedOperation[j] = stoi(string(numStart, numEnd));
            it = numEnd;
        }
        // perform
        int cnt = parsedOperation[0];
        int src = parsedOperation[1] - 1;
        int dst = parsedOperation[2] - 1;
        moveCrates(stacks[src], stacks[dst], cnt, multipleBatch);
    }
    // format result
    string res;
    for (int j = 0; j < stackNum; j++) {
        res.push_back(stacks[j].back());
    }
    return res;
}

void solveA(const vector<string>& input) {
    cout << "A: ";
    cout << solve(input, 9/*stackNum*/, false /*multipleBatches*/) << endl;
}

void solveB(const vector<string>& input) {
    cout << "B: ";
    cout << solve(input, 9/*stackNum*/, true /*multipleBatches*/) << endl;
}

int main() {
    ifstream in("input.txt");
    const auto input = parseInput(in);
    solveA(input);
    solveB(input);
    return 0;
}