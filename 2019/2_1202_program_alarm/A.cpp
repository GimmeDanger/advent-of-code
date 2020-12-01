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

  string input;
  cin >> input;
  transform(begin(input), end(input), begin(input),
      [] (char ch) {
        return (ch == ',' ? ' ' : ch);
  });
  vector<int> stats;
  stringstream ss(input);
  for (int n = 0; ss >> n; ) {
    stats.push_back(n);
  }

  for (size_t i = 0; i < stats.size(); i += 4) {
    if (stats[i] == 99) {
      break;
    }
    else if (stats[i] == 1) {
      int res = stats[stats[i+1]] + stats[stats[i+2]];
      stats[stats[i+3]] = res;
    }
    else if (stats[i] == 2) {
      int res = stats[stats[i+1]] * stats[stats[i+2]];
      stats[stats[i+3]] = res;
    }
  }
  if (!stats.empty())
    cout << stats[0] << endl;

#if dbg
  cin.close();
  cout << endl;
#endif
  return 0;
}