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

  int lo, hi;
  cin >> lo >> hi;

  int cnt = 0;
  for (int i = lo; i <= hi; i++)
  {
    // construct he number
    vector<int> num;
    {
      int n = i;
      while (n > 0) {
        num.push_back(n % 10);
        n /= 10;
      }
      reverse(begin(num), end(num));
    }
    // Criterion #1
    if (num.size() != 6)
      continue;

    // Criterion #2: never decrease
    {
      bool valid = true;
      for (auto it = next(begin(num)); it != end(num); it++) {
        if (*it < *prev(it))
          valid = false;
      }
      if (!valid)
        continue;
    }

    // Criterion #3: two equal adj
    {
      bool valid = false;
      for (auto it = next(begin(num)); it != end(num); it++) {
        if (*it == *prev(it)) {
          valid = true;
        }
      }
      if (!valid)
        continue;
    }

    cnt++;
  }
  cout << cnt;

#if dbg
  cin.close();
  cout << endl;
#endif
  return 0;
}