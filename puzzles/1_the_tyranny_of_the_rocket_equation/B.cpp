#include <bits/stdc++.h>
using namespace std;

#define FAST_IO()              \
  ios::sync_with_stdio(false); \
  cout.tie(nullptr);           \
  cin.tie(nullptr)

#define dbg 1

long long iter(long long x) {
    return x / 3 - 2;
}

long long solve(unordered_map<long long, long long> &cache, long long x) {
  if (x <= 0)
    return 0;
  auto it = cache.find(x);
  if (it != end(cache))
    return it->second;
  long long res = x + solve(cache, iter(x));
  cache[x] = res;
  return res;
}

int main() {
  FAST_IO();
#if dbg
  cout << endl;
  ifstream cin("input.txt");
#endif

  unordered_map<long long, long long> cache;

  long long total = 0;
  for (long long in = 0; cin >> in; )
    total += solve(cache, iter(in));
  cout << total << endl;

#if dbg
  cin.close();
  cout << endl;
#endif
  return 0;
}