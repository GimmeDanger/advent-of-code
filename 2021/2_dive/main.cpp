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
  string command;
  int x = 0, h = 0, v = 0;
  for (auto& line : input) {
    stringstream ss(line);
    ss >> command >> v;
    if (command == "forward")   x += v;
    else if (command == "down") h += v;
    else if (command == "up")   h -= v;
  }
  cout << "A: ";
  cout << x * h << endl;
}

void solve_B(const vector<string>& input) {
  string command;
  int a = 0, x = 0, h = 0, v = 0;
  for (auto& line : input) {
    stringstream ss(line);
    ss >> command >> v;
    if (command == "forward")   x += v, h += a * v;
    else if (command == "down") a += v;
    else if (command == "up")   a -= v;
  }
  cout << "B: ";
  cout << x * h << endl;
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