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

vector<string> parse(istream& in) {
    string line;
    vector<string> input;
    while(getline(in, line)) {
        input.emplace_back(line);
    }
    return input;
}

using graph = vector<vector<int>>;
using graph_small = unordered_set<int>;
using graph_visited = unordered_map<int, int>;
using stack_path = vector<int>;

void dfs(const graph& adj, const graph_small& is_small,
         graph_visited& visited, stack_path& st,
         int src, int dst, int first_small_revisit_limit,
         int& paths) {

    visited[src]++;
    st.push_back(src);
    for (int nxt : adj[src]) {
        if (nxt == dst) {
            paths++;
//            for (int v : st) {
//                cout << v
//                     << " -> ";
//            }
//            cout << endl;
        }
        else {
            bool could_be_visited = true;
            int limit = first_small_revisit_limit;
            if (visited.count(nxt) && is_small.count(nxt)) {
                if (limit > 1 && nxt != 0 && nxt != dst)
                    limit--;
                else
                    could_be_visited = false;
            }

            if (could_be_visited)
                dfs(adj, is_small, visited, st, nxt, dst, limit, paths);
        }
    }
    auto it = visited.find(src);
    if (--(it->second) == 0)
        visited.erase(it);
    st.pop_back();
}

int solve(const vector<string> &input, int first_small_revisit_limit) {
    graph adj;
    graph_small is_small;
    graph_visited visited;
    stack_path st;

    unordered_map<string, int> mp;
    auto insert_vertex = [&mp, &adj, &is_small](const string& v) {
      auto it = mp.find(v);
      if (it == mp.end()) {
          it = mp.insert({v, adj.size()}).first;
          adj.push_back(vector<int>());
      }
      if (islower(v.front())) {
          is_small.insert(it->second);
      }

      return it;
    };

    for (auto& s : input) {
        auto edge = split(s, '-');
        auto from = edge[0];
        auto to = edge[1];
        int v = insert_vertex(from)->second;
        int w = insert_vertex(to)->second;
        adj[v].push_back(w);
        adj[w].push_back(v);
    }
    int paths = 0;
    dfs(adj, is_small, visited, st, mp["start"], mp["end"], first_small_revisit_limit, paths);
    return paths;
}

void solve_A(const vector<string>& input) {
    cout << "A: ";
    cout << solve(input, 1 /*small_edges_visit_limit*/) << endl;
}

void solve_B(const vector<string>& input) {
    cout << "B: ";
    cout << solve(input, 2 /*small_edges_visit_limit*/) << endl;
}

int main() {
    ifstream in("input.txt");
    const auto input = parse(in);
    solve_A(input);
    solve_B(input);
    return 0;
}