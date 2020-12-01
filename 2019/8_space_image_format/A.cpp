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

  const int rows = 6;
  const int cols = 25;
  vector<array<int, 3>> stats;
  int layer_size = 0;
  for (char ch = '\0'; cin >> ch; layer_size++) {
    if (layer_size % (rows * cols) == 0)
      stats.push_back({0, 0, 0});
    int d = ch - '0';
    if (d >= 0 && d <= 2)
      stats.back()[d]++;
  }

  auto it = min_element(begin(stats), end(stats), [](auto &lhs, auto &rhs) { return lhs[0] < rhs[0];} );
  cout << (*it)[1] * (*it)[2] << endl;

#if dbg
  cin.close();
  cout << endl;
#endif
  return 0;
}
