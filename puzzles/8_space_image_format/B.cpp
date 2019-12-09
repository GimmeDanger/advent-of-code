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
  vector<vector<int>> layers(1, vector<int>());
  {
    for (char ch = '\0'; cin >> ch; ) {
      if (layers.back().size () == (rows * cols)) {
        layers.emplace_back();
      }
      int d = ch - '0';
      if (d >= 0 && d <= 2)
        layers.back().push_back(d);
    }
  }
  reverse(begin(layers), end(layers));

  vector<int> answer (rows * cols, 2);
  for (const auto &l : layers) {
    assert(l.size() == cols * rows);
    for (size_t i = 0; i < l.size(); i++) {
      if (l[i] != 2)
        answer[i] = l[i];
    }
  }

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      cout << (answer[r * cols + c] == 1 ? '*' : ' ');
    }
    cout << endl;
  }

  // My secret code: CYKBY
  //   **  *   **  * ***  *   *
  //  *  * *   ** *  *  * *   *
  //  *     * * **   ***   * *
  //  *      *  * *  *  *   *
  //  *  *   *  * *  *  *   *
  //   **    *  *  * ***    *

#if dbg
  cin.close();
  cout << endl;
#endif
  return 0;
}
