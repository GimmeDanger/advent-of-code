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

bool operator==(const object& lhs, const object& rhs) {
  return lhs.position == rhs.position && lhs.velocity == rhs.velocity;
}

int main() {
  FAST_IO();
#if dbg
  cout << endl;
  ifstream cin("input.txt");
#endif

  // Initialize
  vector<object> moons;
  for (int i = 0; i < 4; i++) {
    int x, y, z;
    cin >> x >> y >> z;
    moons.emplace_back (x, y, z);
  }
  const vector<object> initial_moons = moons;

  // dims could be updated separately
  auto update_dimension = [&moons](size_t dim) {
    assert(dim < object::dims);
    // apply gravity
    for (size_t i = 0; i < moons.size(); i++) {
        for (size_t j = i + 1; j < moons.size(); j++) {
            if (moons[i].position[dim] == moons[j].position[dim])
              continue;
            moons[i].velocity[dim] +=
              (moons[i].position[dim] < moons[j].position[dim]) ? +1 : -1;
            moons[j].velocity[dim] +=
              (moons[j].position[dim] < moons[i].position[dim]) ? +1 : -1;
          }
      }
    // apply velocity
    for (auto &m : moons) {
        m.position[dim] += m.velocity[dim];
      }
  };

  // check loop condition
  auto loop_found = [&moons, &initial_moons](size_t dim) {
    for (size_t i = 0; i < moons.size(); i++)
      if (moons[i].position[dim] != initial_moons[i].position[dim] || moons[i].velocity[dim])
        return false;
    return true;
  };

  // find loop period for each dim
  array<unsigned, object::dims> periods{};
  for (size_t k = 0; k < object::dims; k++) {
    do {
        update_dimension (k);
        periods[k]++;
    } while (!loop_found(k));
  }

  // the lcm is a result
  auto lcm = [](long long a, long long b) {
    assert(a > 0 && b > 0);
    return abs(a * b) / __gcd(a, b);
  };
  cout << lcm(lcm(periods[0], periods[1]), periods[2]) << endl;

#if dbg
  cin.close();
  cout << endl;
#endif
  return 0;
}
