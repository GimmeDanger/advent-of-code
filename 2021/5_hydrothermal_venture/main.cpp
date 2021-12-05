#include <iostream>
#include <utility>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <deque>
#include <queue>
#include <map>
#include <cassert>
#include <random>
#include <algorithm>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
using namespace std;

template<typename Cond>
int solve(const vector<string>& input, Cond cond) {
  int res = 0;
  unordered_map<int, unordered_map<int, int>> board;
  for (auto& line : input) {
    stringstream ss(line);
    int x1, x2, y1, y2;
    char ch;
    ss >> x1 >> ch >> y1;
    ss >> ch >> ch;
    ss >> x2 >> ch >> y2;
    if (cond(x1, y1, x2, y2)) {
      int d_x = x2 - x1;
      int d_y = y2 - y1;
      d_x = d_x != 0 ? d_x / abs(d_x) : 0;
      d_y = d_y != 0 ? d_y / abs(d_y) : 0;
      for(; abs(x1 - x2) >= 0 || abs(y1 - y2) >= 0; x1 += d_x, y1 += d_y) {
        auto& b_ij = board[x1][y1];
        if (b_ij == 1) {
          res++;
        }
        b_ij++;
        if (abs(x1 - x2) == 0 && abs(y1 - y2) == 0)
          break;
      }
    }
  }
  return res;
}

void solve_A(const vector<string>& input) {
  cout << "A: ";
  // only consider horizontal and vertical lines
  cout << solve(input, [](int x1, int y1, int x2, int y2) { return x1 == x2 || y1 == y2; }) << endl;
}

void solve_B(const vector<string>& input) {
  cout << "B: ";
  cout << solve(input, [](int, int, int, int) { return true; }) << endl;
}

vector<string> parse(istream& in) {
  string line;
  vector<string> input;
  while(getline(in, line)) {
    input.emplace_back(line);
  }
  return input;
}

int main() {
  ifstream in("input.txt");
  const auto input = parse(in);
  solve_A(input);
  solve_B(input);
  return 0;
}