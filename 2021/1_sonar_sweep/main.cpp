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

void solve_A(const vector<string>& input) {
  int res = 0, prev = -1;
  for (auto& line : input) {
    int x = stoi(line);
    if (prev >= 0 && x > prev)
      res++;
    prev = x;
  }
  cout << "A: " << res << endl;
}

void solve_B(const vector<string>& input) {
  queue<int> q;
  constexpr int WINDOW = 3;
  int sum = 0, res = 0;
  for (auto& line : input) {
    int x = stoi(line);
    if (q.size() == WINDOW) {
      int prev = sum;
      sum -= q.front(); q.pop();
      sum += x; q.push(x);
      if (sum > prev)
        res++;
    }
    else {
      sum += x;
      q.push(x);
    }
  }
  cout << "B: " << res << endl;
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