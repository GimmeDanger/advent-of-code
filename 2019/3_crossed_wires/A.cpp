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
  Point(int x_, int y_) : x(x_), y(y_) {}
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
    Point prev = Point(0, 0);
    for (string s; ss >> s;) {
      char direction = s[0];
      int value = stoi(s.substr(1));
      switch (direction) {
        case 'R':
          for (int x = prev.x + 1, y = prev.y, n = value; n > 0; x++, n--)
            pts.emplace(x, y);
          prev = Point(prev.x + value, prev.y);
          break;
        case 'L':
          for (int x = prev.x - 1, y = prev.y, n = value; n > 0; x--, n--)
            pts.emplace(x, y);
          prev = Point(prev.x - value, prev.y);
          break;
        case 'U':
          for (int x = prev.x, y = prev.y + 1, n = value; n > 0; y++, n--)
            pts.emplace(x, y);
          prev = Point(prev.x, prev.y + value);
          break;
        case 'D':
          for (int x = prev.x, y = prev.y - 1, n = value; n > 0; y--, n--)
            pts.emplace(x, y);
          prev = Point(prev.x, prev.y - value);
          break;
        default:
          break;
      }
    }
  }

  const set<Point> &get_pts() const { return pts; };
};

static int distance(const Point& lhs, const Point& rhs) {
  return abs(lhs.x - rhs.x) + abs(lhs.y - rhs.y);
}

int main() {
  FAST_IO();
#if dbg
  cout << endl;
  ifstream cin("input.txt");
#endif

  vector<Wire> w;
  for (string s; cin >> s;)
    w.emplace_back(move(s));

  Point center(0, 0);
  pair<Point, bool> res(Point(0, 0), false);
  for (size_t i = 0; i < w.size(); i++) {
    for (size_t j = i + 1; j < w.size(); j++) {
      set<Point> s1 = w[i].get_pts();
      set<Point> s2 = w[j].get_pts();
      vector<Point> inter;
      set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), back_inserter(inter));
      auto it =
          min_element(begin(inter), end(inter),
                      [&center](const auto &lhs, const auto &rhs) {
                        return distance(lhs, center) < distance(rhs, center);
                      });
      if (it != end(inter) &&
          (!res.second ||
              distance(*it, center) < distance(res.first, center))) {
        res.first = *it;
        res.second = true;
      }
    }
  }

  if (res.second) {
    cout << "Distance to closest intersection = " << distance(res.first, center)
         << endl;
  } else {
    cout << "No intersection found" << endl;
  }

#if dbg
  cin.close();
  cout << endl;
#endif
  return 0;
}