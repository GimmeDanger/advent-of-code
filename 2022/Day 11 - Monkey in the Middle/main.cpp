#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <queue>
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

constexpr int MONKEYS = 7;
constexpr int ROUNDS = 20;

class MonkeyStateMachine {
    queue<long long> items;
    function<long long(long long)> op;
    function<long long(int)> test;
    int inspectedCnt = 0;
public:
    MonkeyStateMachine(const vector<long long>& _items, function<int(int)> _op, function<int(int)> _test)
        : op(_op), test(_test)
    {
        for (long long n : _items) {
            items.push(n);
        }
    }

    void print() const {
        cout << "Inspected cnt: " << inspectedCnt << endl;
    }

    bool couldThrow() const {
        return !items.empty();
    }

    void get(long long item) {
        items.push(item);
    }

    pair<int, long long> throwTo() {
        assert(couldThrow());
        inspectedCnt++;
        long long fr = items.front(); items.pop();
        long long worry_level = op(fr) / 3;
        int item_dst = test(worry_level);
        return {item_dst, worry_level};
    }
};

void solveA(const vector<string>& input) {
    cout << "A: " << endl;
    vector<MonkeyStateMachine> monkeys = {
//            MonkeyStateMachine({79, 98}, [](int v) { return v * 19;}, [](int level) { return level % 23 == 0 ? 2 : 3; }),
//            MonkeyStateMachine({54, 65, 75, 74}, [](int v) { return v + 6;}, [](int level) { return level % 19 == 0 ? 2 : 0; }),
//            MonkeyStateMachine({79, 60, 97}, [](int v) { return v * v;}, [](int level) { return level % 13 == 0 ? 1 : 3; }),
//            MonkeyStateMachine({74}, [](int v) { return v + 3;}, [](int level) { return level % 17 == 0 ? 0 : 1; })
            MonkeyStateMachine({65, 78}, [](int v) { return v * 3;}, [](int level) { return level % 5 == 0 ? 2 : 3; }),
            MonkeyStateMachine({54, 78, 86, 79, 73, 64, 85, 88}, [](int v) { return v + 8;}, [](int level) { return level % 11 == 0 ? 4 : 7; }),
            MonkeyStateMachine({69, 97, 77, 88, 87}, [](int v) { return v + 2;}, [](int level) { return level % 2 == 0 ? 5 : 3; }),
            MonkeyStateMachine({99}, [](int v) { return v + 4;}, [](int level) { return level % 13 == 0 ? 1 : 5; }),
            MonkeyStateMachine({60, 57, 52}, [](int v) { return v * 19;}, [](int level) { return level % 7 == 0 ? 7 : 6; }),
            MonkeyStateMachine({91, 82, 85, 73, 84, 53}, [](int v) { return v + 5;}, [](int level) { return level % 3 == 0 ? 4 : 1; }),
            MonkeyStateMachine({88, 74, 68, 56}, [](int v) { return v * v;}, [](int level) { return level % 17 == 0 ? 0 : 2; }),
            MonkeyStateMachine({54, 82, 72, 71, 53, 99, 67}, [](int v) { return v + 1;}, [](int level) { return level % 19 == 0 ? 6 : 0; })
    };

    for (int r = 1; r <= ROUNDS; r++) {
        for (auto& m : monkeys) {
            while (m.couldThrow()) {
                auto [id, item] = m.throwTo();
                cout << id << " " << item << endl;
                monkeys[id].get(item);
            }
        }
    }
    for (auto& m : monkeys) {
        m.print();
    }
}

void solveB(const vector<string>& input) {
    cout << "B: " << endl;
}

int main() {
    ifstream in("input.txt");
    const auto input = parseInput(in);
    solveA(input);
    solveB(input);
    return 0;
}