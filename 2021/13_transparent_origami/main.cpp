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

void split_coords(const string& s, char delimiter, unordered_map<int, unordered_set<int>>& coords_store) {
    auto pos = s.find(delimiter);
    if (pos != string::npos) {
        int x = stoi(s.substr(0, pos));
        int y = stoi(s.substr(pos + 1));
        auto [it, inserted] = coords_store.emplace(x, unordered_set<int>());
        it->second.insert(y);
    }
}

void split_folds(const string& s, char delimiter, vector<pair<char, int>>& folds_store) {
    auto pos = s.find(delimiter);
    if (pos != string::npos) {
        int val = stoi(s.substr(pos + 1));
        if (s[pos - 1] == 'x')
            folds_store.emplace_back(1, val);
        else
            folds_store.emplace_back(0, val);
    }
}

vector<string> parse(istream& in) {
    string line;
    vector<string> input;
    while(getline(in, line)) {
        input.emplace_back(line);
    }
    return input;
}

void print_grid(const unordered_map<int, unordered_set<int>>& coords_store) {
    int min_rows = 0, min_cols = 0;
    int max_rows = 0, max_cols = 0;
    for (auto& [x, y_set] : coords_store) {
        max_cols = max(max_cols, x);
        min_cols = min(min_cols, x);
        for (int y : y_set) {
            max_rows = max(max_rows, y);
            min_rows = min(min_rows, y);
        }
    }
    for (int r = min_rows; r <= max_rows; r++) {
        for (int c = min_cols; c <= max_cols; c++) {
            auto it = coords_store.find(c);
            if (it != coords_store.end() && it->second.count(r) != 0)
                cout << "#";
            else
                cout << ".";
        }
        cout << endl;
    }
    cout << endl;
}

void fold_iteration(int fold_val, int fold_type, unordered_map<int, unordered_set<int>>& coords_store) {
    unordered_map<int, unordered_set<int>> new_coords_store;
    auto transform = [fold_val, fold_type](int x, int y) -> pair<int, int> {
        if (fold_type) {
            // x
            if (fold_val >= x) return {x, y};
            else               return {fold_val - (x - fold_val), y};
        } else {
            // y
            if (fold_val >= y) return {x, y};
            else               return {x, fold_val - (y - fold_val)};
        }
    };
    for (auto& [x, y_set] : coords_store) {
        for (int y : y_set) {
            auto [new_x, new_y] = transform(x, y);
            auto [it, inserted] = new_coords_store.emplace(new_x, unordered_set<int>());
            it->second.insert(new_y);
        }
    }
    coords_store = new_coords_store;
}

int solve(const vector<string> &input, bool first_fold_only, bool verbose = false) {
    unordered_map<int, unordered_set<int>> coords_store;
    vector<pair<char, int>> folds_store;
    for (const auto& line : input) {
        if (line.find(',') != string::npos) {
            split_coords(line, ',', coords_store);
        }
        else {
            split_folds(line, '=', folds_store);
            if (!folds_store.empty() && first_fold_only)
                break;
        }
    }
    for (auto& [type, val] : folds_store) {
        // print_grid(coords_store);
        fold_iteration(val, type, coords_store);

    }
    if (verbose)
        print_grid(coords_store);
    int dots = 0;
    for (auto& [x, y_set] : coords_store)
        dots += y_set.size();
    return dots;
}

void solve_A(const vector<string>& input) {
    cout << "A:" << endl;
    cout << solve(input, true) << endl;
}

void solve_B(const vector<string>& input) {
    cout << "B:" << endl;
    cout << solve(input, false, true) << endl;
}

int main() {
    ifstream in("input.txt");
    const auto input = parse(in);
    solve_A(input);
    solve_B(input);
    return 0;
}