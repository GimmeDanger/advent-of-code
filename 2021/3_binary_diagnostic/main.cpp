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
  vector<int> cnt(input.front().size(), 0);
  for (auto& line : input) {
    for (unsigned i = 0; i < line.size(); i++)
      if (line[i] == '1')
        cnt[i]++;
  }
  int N = input.size();
  string gamma_binary(input.front().size(), '0');
  string eps_binary(input.front().size(), '1');
  for (unsigned i = 0; i < cnt.size(); i++) {
    if (cnt[i] > N / 2) {
      gamma_binary[i] = '1';
      eps_binary[i] = '0';
    }
  }
  int gamma = std::stoi(gamma_binary, nullptr, 2);
  int eps = std::stoi(eps_binary, nullptr, 2);
  cout << "A: ";
  cout << gamma * eps << endl;
}

template<typename Cond>
void partition_out(vector<string>& candidates, int pos, Cond cond_func) {
  int N = candidates.size();
  if (N <= 1)
    return;
  int cnt = 0;
  for (const auto& s : candidates) {
    if (s[pos] == '1')
      cnt++;
  }
  int cnd = cond_func(cnt, N);
  auto it = partition(
      candidates.begin(), candidates.end(),
      [pos, cnd](auto& s) { return s[pos] == (cnd ? '1' : '0'); }
  );
  candidates.erase(it, candidates.end());
}

void solve_B(const vector<string>& input) {
  unsigned bits = input.front().size();
  auto oxygen_candidates = input;
  auto co2_candidates = input;
  for (unsigned i = 0; i < bits; i++) {
    partition_out(oxygen_candidates, i, [](int cnt, int N) { return 2 * cnt >= N; });
    partition_out(co2_candidates, i, [](int cnt, int N) { return 2 * (N - cnt) > N; });
  }
  assert(oxygen_candidates.size() == 1);
  assert(co2_candidates.size() == 1);
  int oxy = std::stoi(oxygen_candidates.front(), nullptr, 2);
  int co2 = std::stoi(co2_candidates.front(), nullptr, 2);
  cout << "B: ";
  cout << oxy * co2 << endl;
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