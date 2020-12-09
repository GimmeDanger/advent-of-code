#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <optional>
#include <array>
#include <cctype>
#include <cstring>
#include <string>
#include <string_view>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <charconv>
#include <algorithm>

using namespace std;

#define dbg 1

#if dbg
#define INPUT_SOURCE() ifstream cin("input.txt")
#else
#define INPUT_SOURCE()
#endif

#define FAST_IO()              \
  ios::sync_with_stdio(false); \
  cout.tie(nullptr);           \
  cin.tie(nullptr)

using edge = pair<int, int>;
using graph = vector<vector<edge>>;
using lookup_index = unordered_map<string, int>;

void parse_bag_color(string_view line, string &color, int &count) {

    auto pos = line.find(' ');
    if (pos == string::npos) {
        throw std::invalid_argument("Cannot parse bag color");
    }

    // parse count
    auto possible_number = line.substr(0, pos);
    auto result = from_chars(line.data(), line.data() + pos, count);
    line = line.substr(pos + 1);
    if (result.ec == std::errc::invalid_argument) {
        count = -1;               //< it must be a start of color
        color = possible_number;  //< initialize the start
    } else {
        pos = line.find(' ');
        if (pos == string::npos) {
            throw std::invalid_argument("Cannot parse bag color");
        }
        color = line.substr(0, pos); //< initialize the start
        line = line.substr(pos + 1);
    }

    // color start has been initialized
    color += ' ';
    pos = line.find(' ');
    if (pos == string::npos) {
        throw std::invalid_argument("Cannot parse bag color");
    }
    color += line.substr(0, pos);
}

int get_color_id(string color, lookup_index& color_index) {
    auto it = color_index.find(color);
    if (it == color_index.end()) {
        int ret = color_index.size();
        color_index.emplace(move(color), ret);
        return ret;
    }
    return it->second;
}

void add_edge(int from, int to, int weight, graph &adj) {
    while (adj.size() <= from) {
        adj.push_back({});
    }
    adj[from].emplace_back(to, weight);
}

// Format <color> bag contains x <color> bags,
void parse(const string_view line, graph& adj, lookup_index& color_index) {
    auto pos = line.find("contain");
    if (pos == string::npos)
        return;

    int count{0};
    string bag_color;
    auto prefix = line.substr(0, pos);
    parse_bag_color(prefix, bag_color, count);
    int from = get_color_id(bag_color, color_index);

    auto suffix = line.substr(pos + strlen("contain") + 1);
    for (auto i = 0, j = 0; j < suffix.size(); j++) {
        if (suffix[j] == '.' || suffix[j] == ',') {
            parse_bag_color(suffix.substr(i, j - i), bag_color, count);
            if (bag_color != "no other") {
                int to = get_color_id(bag_color, color_index);
                add_edge(from, to, count, adj);
            }
            i = j + 2;
        }
    }
}

bool reachable(const graph &adj, vector<bool> &visited, int s, int t) {
    visited[s] = true;
    if (s == t) {
        return true;
    }
    for (auto &e : adj[s]) {
        int v = e.first;
        if (!visited[v] && reachable(adj, visited, v, t)) {
            return true;
        }
    }
    return false;
}

size_t count_sub_bugs(const graph &adj, int s) {
    size_t sub_bags = 0;
    for (auto &e : adj[s]) {
        int v = e.first, w = e.second;
        sub_bags += w * count_sub_bugs(adj, v);
    }
    return sub_bags + 1;
}


void solve(istream &is, ostream &os) {

    graph adj;
    lookup_index color_index;
    for (string line; getline(is, line); ) {
        parse(line, adj, color_index);
    }
    while (adj.size() <= color_index.size()) {
        adj.push_back({});
    }

    // A: How many bag colors can eventually contain at least one shiny gold bag?
    // Just dfs from each vertex to check reachability
    size_t count = 0;
    const int t = color_index.at("shiny gold");
    vector<bool> visited(adj.size());
    for (int s = 0; s < adj.size(); s++) {
        fill(begin(visited), end(visited), false);
        if (reachable(adj, visited, s, t)) {
            count++;
        }
    }
    os << "A: " << count - 1 << endl;

    // B: How many individual bags are required inside your single shiny gold bag?
    os << "B: " << count_sub_bugs(adj, t) - 1 << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
