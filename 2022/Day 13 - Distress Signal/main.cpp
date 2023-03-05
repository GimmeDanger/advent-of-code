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

vector<string> split(string s, string delimiter) {
    size_t pos_start = 0, pos_end;
    string token;
    vector<string> res;
    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delimiter.size();
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

struct Packet {
    vector<Packet> lst;
    optional<int> val;
    string input;

    Packet() = default;

    Packet(int _val) {
        val = _val;
    }

    Packet(vector<Packet> _lst) {
        lst = _lst;
    }

    // parse with stack
    Packet(const string& line) {
        stack<vector<Packet>> st;
        for (int i = 0; i < line.size(); i++) {
            char ch = line[i];
            if (ch == '[') {
                st.push({});
            }
            else if (isdigit(ch)) {
                auto& last = st.top();
                if (last.empty() || line[i - 1] == ',') {
                    last.push_back(Packet(ch - '0'));
                } else {
                    last.back().val = last.back().val.value() * 10 + (ch - '0');
                }
            }
            else if (ch == ']') {
                auto last = st.top(); st.pop();
                if (!st.empty())
                    st.top().push_back(Packet(last));
                else
                    lst = last;
            }
        }
        input = line;
    }

    int operator<(const Packet& other) {
        if (this->val.has_value() && other.val.has_value()) {
            return this->val.value() < other.val.value() ? 1 : this->val.value() == other.val.value() ? 0 : -1;
        }
        else if (this->val.has_value()) {
            int v = this->val.value();
            Packet converted(vector<Packet>(1, Packet(v)));
            return converted < other;
        }
        else if (other.val.has_value()) {
            int v = other.val.value();
            Packet converted(vector<Packet>(1, Packet(v)));
            return *this < converted;
        }
        else {
            for (int i = 0; i < max(lst.size(), other.lst.size()); i++) {
                if (lst.size() != other.lst.size()) {
                    if (i == lst.size()) {
                        return 1;
                    }
                    else if (i == other.lst.size()) {
                        return -1;
                    }
                }
                if (int cmpr = lst[i] < other.lst[i]; cmpr != 0) {
                    return cmpr;
                }
            }
            return 0;
        }
    }
};

int solveAImpl(const vector<string>& input) {
    int res = 0;
    for (int i = 0; i < input.size(); i += 3) {
        auto lo = Packet(input[i]);
        auto hi = Packet(input[i + 1]);
        if ((lo < hi) == 1) {
            res += (i / 3 + 1);
        }
    }
    return res;
}

void solveBImpl(const vector<string>& input) {
    vector<Packet> packets;
    for (int i = 0; i < input.size(); i += 3) {
        packets.push_back(Packet(input[i]));
        packets.push_back(Packet(input[i + 1]));
    }
    packets.push_back(Packet("[[2]]"));
    packets.push_back(Packet("[[6]]"));
    sort(begin(packets), end(packets), [](auto& lo, auto& hi) {
        return lo < hi == 1;
    });
    for (int i = 0; i < packets.size(); i++) {
        cout << i + 1 << " " << packets[i].input << endl;
    }
}

void solveA(const vector<string>& input) {
    cout << "A: " << endl;
    cout << solveAImpl(input) << endl;
}

void solveB(const vector<string>& input) {
    cout << "B: " << endl;
    solveBImpl(input);
}

int main() {
    ifstream in("input.txt");
    const auto input = parseInput(in);
    solveA(input);
    solveB(input);
    return 0;
}