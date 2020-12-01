#include <bits/stdc++.h>
using namespace std;

#define FAST_IO()                                                              \
  ios::sync_with_stdio(false);                                                 \
  cout.tie(nullptr);                                                           \
  cin.tie(nullptr)

#define dbg 1

class Intcode {
  const vector<int> initial_memory;

  static vector<int> parse(string initial_state) {
    transform(
        begin(initial_state), end(initial_state), begin(initial_state),
        [] (char ch) {
          return (ch == ',' ? ' ' : ch);
        });
    stringstream ss(initial_state);
    vector<int> tmp_memory;
    for (int n = 0; ss >> n; ) {
      tmp_memory.push_back(n);
    }
    return tmp_memory;
  }

  static int eval(vector<int> &memory) {
    for (size_t i = 0; i < memory.size(); i += 4) {
      if (memory[i] == 99) {
        break;
      }
      else if (memory[i] == 1) {
        int res = memory[memory[i+1]] + memory[memory[i+2]];
        memory[memory[i+3]] = res;
      }
      else if (memory[i] == 2) {
        int res = memory[memory[i+1]] * memory[memory[i+2]];
        memory[memory[i+3]] = res;
      }
    }
    return memory[0];
  }

public:
  Intcode(const string &initial_state) : initial_memory(parse(initial_state)) { }

  int eval(const int noun, const int verb) {
    auto test = initial_memory;
    test[1] = noun;
    test[2] = verb;
    return eval(test);
  }
};

int main() {
  FAST_IO();

#if dbg
  cout << endl;
  ifstream cin("input.txt");
#endif

  string initial_state;
  cin >> initial_state;
  Intcode cpu(initial_state);
  for (int noun = 0; noun < 100; noun++)
    for (int verb = 0; verb < 100; verb++)
      if (cpu.eval(noun, verb) == 19690720) {
        cout << 100 * noun + verb << endl;
        break;
      }

#if dbg
  cin.close();
  cout << endl;
#endif
  return 0;
}