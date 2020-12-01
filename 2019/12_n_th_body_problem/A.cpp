#include <bits/stdc++.h>
using namespace std;

#define FAST_IO()                                                              \
  ios::sync_with_stdio(false);                                                 \
  cout.tie(nullptr);                                                           \
  cin.tie(nullptr)

#define dbg 1

struct object {
  static constexpr int dims = 3;
  array<int, dims> position{};
  array<int, dims> velocity{};
  object (int _x, int _y, int _z)
  : position({_x, _y, _z}) { }
};

int main() {
  FAST_IO();
#if dbg
  cout << endl;
  ifstream cin("input.txt");
#endif

  vector<object> moons;
  for (int i = 0; i < 4; i++) {
    int x, y, z;
    cin >> x >> y >> z;
    moons.emplace_back (x, y, z);
  }

  for (int t = 0; t < 1000; t++) {
    // apply gravity
    for (size_t i = 0; i < moons.size (); i++) {
        for (size_t j = i + 1; j < moons.size(); j++) {
          for (size_t k = 0; k < object::dims; k++) {
            if (moons[i].position[k] == moons[j].position[k])
              continue;
            moons[i].velocity[k] += (moons[i].position[k] < moons[j].position[k]) ? +1 : -1;
            moons[j].velocity[k] += (moons[j].position[k] < moons[i].position[k]) ? +1 : -1;
          }
        }
    }
    // apply velocity
    for (auto &m : moons) {
        for (size_t k = 0; k < object::dims; k++) {
            m.position[k] += m.velocity[k];
        }
    }
  }

  int total_energy = 0;
  for (auto &m : moons) {
      total_energy += accumulate(begin(m.position), end(m.position), 0, [](int total, int value) { return total + abs (value); }) *
                      accumulate(begin(m.velocity), end(m.velocity), 0, [](int total, int value) { return total + abs (value); });
    }
  cout << total_energy << endl;

#if dbg
  cin.close();
  cout << endl;
#endif
  return 0;
}
