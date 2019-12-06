#include <bits/stdc++.h>
using namespace std;

#define FAST_IO()                                                              \
  ios::sync_with_stdio(false);                                                 \
  cout.tie(nullptr);                                                           \
  cin.tie(nullptr)

#define dbg 1

int main() {
  FAST_IO();
#if dbg
  cout << endl;
  ifstream cin("input.txt");
#endif

  vector<vector<int>> adj;
  unordered_map<string, int> mp;

  // decode helper: O(1)
  auto get_id = [&adj, &mp] (string s) {
    auto it = mp.find(s);
    if (it != end(mp))
      return it->second;
    int id = adj.size();
    mp[std::move(s)] = id;
    adj.emplace_back();
    return id;
  };

  // parse input
  for (string s; cin >> s; ) {
    auto from = get_id(s.substr(0, 3));
    auto to = get_id(s.substr(4, 6));
    adj[from].push_back(to);
  }

  // single source bfs
  const int s = mp["COM"];
  vector<int> dist(adj.size(), numeric_limits<int>::max());
  vector<bool> visited(adj.size(), false);
  queue<int> q;

  // initialize
  q.push(s);
  dist[s] = 0;

  // input is guaranteed to have only 1 cc
  while (!q.empty ()) {
    auto v = q.front(); q.pop();
    visited[v] = true;
    for (auto w : adj[v]) {
      if (!visited[w]) {
        q.push(w);
        dist[w] = dist[v] + 1;
      }
    }
  }

  cout << "Total orbit distances = ";
  cout << std::accumulate(begin(dist), end(dist), 0ll) << endl;

#if dbg
  cin.close();
  cout << endl;
#endif
  return 0;
}