#include <bits/stdc++.h>
using namespace std;

#define FAST_IO()              \
  ios::sync_with_stdio(false); \
  cout.tie(nullptr);           \
  cin.tie(nullptr)

#define dbg 1

int main() {
  FAST_IO();
#if dbg
  cout << endl;
  ifstream cin("input.txt");
#endif

  long long total = 0;
  for (long long in = 0; cin >> in; )
    total += (in / 3 - 2);
  cout << total << endl;

#if dbg
  cin.close();
  cout << endl;
#endif
  return 0;
}