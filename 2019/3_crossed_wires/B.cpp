#include <bits/stdc++.h>
using namespace std;

#define FAST_IO()                                                              \
  ios::sync_with_stdio(false);                                                 \
  cout.tie(nullptr);                                                           \
  cin.tie(nullptr)

#define dbg 1

struct Point {
  int x = 0;
  int y = 0;
  int steps = 0;
  Point(int x_, int y_, int steps_) : x(x_), y(y_), steps(steps_) {}
};

bool operator<(const Point& lhs, const Point& rhs) {
  return tie(lhs.x, lhs.y) < tie(rhs.x, rhs.y);
}

class Wire {
  set<Point> pts;

 public:
  explicit Wire(string input) {
    replace(begin(input), end(input), ',', ' ');
    stringstream ss(input);
    Point prev = Point(0, 0, 0);
    auto update = [this, &prev](int x, int y, int steps) {
      pts.emplace(x, y, steps);
      prev = Point(x, y, steps);
    };
    for (string s; ss >> s;) {
      char direction = s[0];
      int value = stoi(s.substr(1));
      for (int x = prev.x, y = prev.y, steps = prev.steps, n = value; n > 0; n--) {
        switch (direction) {
          case 'R':
            update(++x, y, ++steps);
            break;
          case 'L':
            update(--x, y, ++steps);
            break;
          case 'U':
            update(x, ++y, ++steps);
            break;
          case 'D':
            update(x, --y, ++steps);
            break;
          default:
            break;
        }
      }
    }
  }

  const set<Point> &get_pts() const { return pts; };
};

int main() {
  FAST_IO();
#if dbg
  cout << endl;
  ifstream cin("input.txt");
#endif

  vector<Wire> w;
  for (string s; cin >> s;)
    w.emplace_back(move(s));

  for (size_t i = 0; i < w.size(); i++) {
    for (size_t j = i + 1; j < w.size(); j++) {
      vector<int> inter;
      for (auto &p : w[i].get_pts()) {
        auto it = w[j].get_pts().find(p);
        if (it != end(w[j].get_pts()))
          inter.push_back(p.steps + it->steps);
      }

      auto it = min_element(begin(inter), end(inter));
      if (it != end(inter))
        cout << *it << endl;
    }
  }

#if dbg
  cin.close();
  cout << endl;
#endif
  return 0;
}