#include <bits/stdc++.h>
using namespace std;

#define FAST_IO()                                                              \
  ios::sync_with_stdio(false);                                                 \
  cout.tie(nullptr);                                                           \
  cin.tie(nullptr)

#define dbg 1

constexpr int max_arg_num = 3;
constexpr int args_offset = 1;
constexpr int special_value = numeric_limits<int>::max();

enum class command {
  halt = 99,
  add = 1,
  mult = 2,
  input = 3,
  output = 4
};

static int ARGS_NUM(command cmd) {
  switch (cmd) {
    case command::halt:
      return 0;
    case command::add:
    case command::mult:
      return 3;
    case command::input:
    case command::output:
      return 1;
    default:
      return -1;
  }
}

enum class parameter_mode {
  position = 0,
  immediate = 1
};

template<typename T>
static bool EQUAL(int code, T cmd) {
  return code == static_cast<int>(cmd);
}

static auto parse_instr(int instr_code) {
  // parse instr
  int instr = instr_code % 100;
  int arg_num = ARGS_NUM(static_cast<command>(instr));
  instr_code /= 100;
  // parse arg modes
  std::array<int, max_arg_num> arg_modes{};
  for (size_t pos = 0; instr_code; instr_code /= 10, pos++)
    arg_modes[pos] = instr_code % 10;
  return make_tuple(instr, arg_num, arg_modes);
}

static int pick_value(const vector<int>& memory, int mode, size_t pos) {
  if (EQUAL(mode, parameter_mode::position))
    return memory[memory[pos]];
  else if (EQUAL(mode, parameter_mode::immediate))
    return memory[pos];
  else
    return -1;
}

static void set_value(vector<int>& memory, int mode, size_t pos, int value) {
  if (EQUAL(mode, parameter_mode::position))
    memory[memory[pos]] = value;
  else if (EQUAL(mode, parameter_mode::immediate))
    memory[pos] = value;
}

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

  static int eval(vector<int> memory, int input) {
    int output = special_value;
    for (size_t i = 0; i < memory.size(); ) {
      // parse instruction and move to carret to arguments
      auto [instr, arg_num, arg_modes] = parse_instr(memory[i]);
      i += args_offset;

      // perform an instruction
      if (EQUAL(instr, command::halt)) {
        break;
      }
      else if (EQUAL(instr, command::add)) {
        int val0 = pick_value(memory, arg_modes[0], i + 0);
        int val1 = pick_value(memory, arg_modes[1], i + 1);
        set_value(memory, arg_modes[2], i + 2, val0 + val1);
      }
      else if (EQUAL(instr, command::mult)) {
        int val0 = pick_value(memory, arg_modes[0], i + 0);
        int val1 = pick_value(memory, arg_modes[1], i + 1);
        set_value(memory, arg_modes[2], i + 2, val0 * val1);
      }
      else if (EQUAL(instr, command::input)) {
        if (input == special_value)
          throw invalid_argument("Only single value input is supported.");
        set_value(memory, arg_modes[0], i + 0, input);
        input = special_value;
      }
      else if (EQUAL(instr, command::output)) {
        output = pick_value(memory, arg_modes[0], i + 0);
      }

      i += arg_num;
    }
    return output;
  }

 public:
  explicit Intcode(const string &initial_state) : initial_memory(parse(initial_state)) { }

  int eval(const int input) {
    return eval(initial_memory, input);
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
  cout << cpu.eval(1) << endl;

#if dbg
  cin.close();
  cout << endl;
#endif
  return 0;
}