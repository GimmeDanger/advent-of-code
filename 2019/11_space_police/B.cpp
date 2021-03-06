#include <bits/stdc++.h>
using namespace std;

#define FAST_IO()                                                              \
  ios::sync_with_stdio(false);                                                 \
  cout.tie(nullptr);                                                           \
  cin.tie(nullptr)

#define dbg 1

constexpr int max_arg_num = 3;
constexpr int instr_offset = 1;
constexpr int special_value = numeric_limits<int>::max();

enum class command {
  halt = 99,
  add = 1,
  mult = 2,
  input = 3,
  output = 4,
  jump_if_true = 5,
  jump_if_false = 6,
  less_than = 7,
  equals = 8,
  adjusts_rbase = 9
};

static int ARGS_NUM(command cmd) {
  switch (cmd) {
    case command::halt:
      return 0;
    case command::add:
    case command::mult:
    case command::less_than:
    case command::equals:
      return 3;
    case command::input:
    case command::output:
    case command::adjusts_rbase:
      return 1;
    case command::jump_if_true:
    case command::jump_if_false:
      return 2;
    default:
      return -1;
  }
}

enum class parameter_mode {
  position = 0,
  immediate = 1,
  relative = 2
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

class Intcode {
  vector<long long> memory;
  bool initialized = false;
  bool halted = false;
  size_t ptr = 0;
  size_t rptr = 0;

  static vector<long long> parse(string initial_state) {
    transform(
        begin(initial_state), end(initial_state), begin(initial_state),
        [] (char ch) {
          return (ch == ',' ? ' ' : ch);
        });
    stringstream ss(initial_state);
    vector<long long> tmp_memory;
    for (long long n = 0; ss >> n; ) {
      tmp_memory.push_back(n);
    }
    return tmp_memory;
  }

  template<size_t arg_num>
  long long pick_value(std::array<int, max_arg_num> &arg_modes, size_t arg_starting_pos) {
    auto mode = arg_modes[arg_num];
    auto pos = arg_starting_pos + arg_num;
    if (EQUAL(mode, parameter_mode::position)) {
      if (pos >= memory.size())
        memory.resize(2 * pos, 0);
      if (memory[pos] >= memory.size())
        memory.resize(2 * memory[pos], 0);
      return memory[memory[pos]];
    }
    else if (EQUAL(mode, parameter_mode::immediate)) {
      if (pos >= memory.size())
        memory.resize(2 * pos, 0);
      return memory[pos];
    }
    else if (EQUAL(mode, parameter_mode::relative)) {
      if (pos >= memory.size())
        memory.resize(2 * pos, 0);
      if (rptr + memory[pos] >= memory.size())
        memory.resize(2 * (rptr + memory[pos]), 0);
      return memory[rptr + memory[pos]];
    }
    else {
      return -1;
    }
  }

  template<size_t arg_num>
  void set_value(std::array<int, max_arg_num> &arg_modes, size_t arg_starting_pos, long long value) {
    auto mode = arg_modes[arg_num];
    auto pos = arg_starting_pos + arg_num;
    if (EQUAL(mode, parameter_mode::position)) {
      if (pos >= memory.size())
        memory.resize(2 * pos, 0);
      if (memory[pos] >= memory.size())
        memory.resize(2 * memory[pos], 0);
      memory[memory[pos]] = value;
    }
    else if (EQUAL(mode, parameter_mode::immediate)) {
      if (pos >= memory.size())
        memory.resize(2 * pos, 0);
      memory[pos] = value;
    }
    else if (EQUAL(mode, parameter_mode::relative)) {
      if (pos >= memory.size())
        memory.resize(2 * pos), 0;
      if (rptr + memory[pos] >= memory.size())
        memory.resize(2 * (rptr + memory[pos]), 0);
      memory[rptr + memory[pos]] = value;
    }
  }

 public:
  explicit Intcode(const string &initial_state) : memory(parse(initial_state)) { }

  bool is_halt() const {
    return halted;
  }

  bool is_initialized() const {
    return initialized;
  }

  vector<long long> eval(vector<long long>::const_iterator first, vector<long long>::const_iterator last) {
    vector<long long> output;
    while (ptr < memory.size() && !is_halt()) {
      // parse instruction and move to carret to arguments
      auto [instr, arg_num, arg_modes] = parse_instr(memory[ptr]);
      size_t pointer_offset = instr_offset + arg_num;
      size_t arg_starting_pos = ptr + instr_offset;

      // perform an instruction
      if (EQUAL(instr, command::halt)) {
        halted = true;
        break;
      }
      else if (EQUAL(instr, command::add)) {
        auto val0 = pick_value<0>(arg_modes, arg_starting_pos);
        auto val1 = pick_value<1>(arg_modes, arg_starting_pos);
        set_value<2>(arg_modes, arg_starting_pos, val0 + val1);
      }
      else if (EQUAL(instr, command::mult)) {
        auto val0 = pick_value<0>(arg_modes, arg_starting_pos);
        auto val1 = pick_value<1>(arg_modes, arg_starting_pos);
        set_value<2>(arg_modes, arg_starting_pos, val0 * val1);
      }
      else if (EQUAL(instr, command::input)) {
        assert(first != last);
        set_value<0>(arg_modes, arg_starting_pos, *first);
        first = std::next(first);
        initialized = true;
      }
      else if (EQUAL(instr, command::output)) {
        assert(first == last);
        output.push_back(pick_value<0>(arg_modes, arg_starting_pos));
      }
      else if (EQUAL(instr, command::jump_if_true)) {
        if (pick_value<0>(arg_modes, arg_starting_pos)) {
          ptr = pick_value<1>(arg_modes, arg_starting_pos);
          pointer_offset = 0;
        }
      }
      else if (EQUAL(instr, command::jump_if_false)) {
        if (!pick_value<0>(arg_modes, arg_starting_pos)) {
          ptr = pick_value<1>(arg_modes, arg_starting_pos);
          pointer_offset = 0;
        }
      }
      else if (EQUAL(instr, command::less_than)) {
        auto val0 = pick_value<0>(arg_modes, arg_starting_pos);
        auto val1 = pick_value<1>(arg_modes, arg_starting_pos);
        set_value<2>(arg_modes, arg_starting_pos, (val0 < val1 ? 1 : 0));
      }
      else if (EQUAL(instr, command::equals)) {
        auto val0 = pick_value<0>(arg_modes, arg_starting_pos);
        auto val1 = pick_value<1>(arg_modes, arg_starting_pos);
        set_value<2>(arg_modes, arg_starting_pos, (val0 == val1 ? 1 : 0));
      }
      else if (EQUAL(instr, command::adjusts_rbase)) {
        rptr += pick_value<0>(arg_modes, arg_starting_pos);
      }

      // update instr pointer
      ptr += pointer_offset;

      // sleep until the next input
      if (EQUAL(instr, command::output) && output.size() == 2)
        break;
    }
    return output;
  }
};

enum class direction {
  U,
  R,
  D,
  L,
  COUNT
};

void rotate(const bool turn_right, direction &dir) {
  int dir_code = static_cast<int>(dir);
  int total = static_cast<int>(direction::COUNT);
  dir_code = (dir_code + (turn_right ? +1 : -1) + total) % total;
  dir = static_cast<direction>(dir_code);
}

void step_forward(const direction dir, pair<int, int> &pos) {
  switch(dir) {
    case direction::R:
      pos.first += 1;
      return;
    case direction::L:
      pos.first -= 1;
      return;
    case direction::U:
      pos.second += 1;
      return;
    case direction::D:
      pos.second -= 1;
      return;
    default:
      return;
  }
}

int main() {
  FAST_IO();

#if dbg
  cout << endl;
  ifstream cin("input.txt");
#endif

  string initial_state;
  cin >> initial_state;
  Intcode cpu(initial_state);

  direction dir = direction::U;
  pair<int, int> pos = {0, 0};
  map<pair<int, int>, int> visited; // <position, color>
  visited.insert({pos, 1 /*black*/});

  while (true) {
    vector<long long> input = {visited.at(pos)};
    auto output = cpu.eval(begin(input), end(input));
    if (cpu.is_halt())
      break;
    // update position
    assert(output.size() == 2);
    visited[pos] = output[0];
    rotate(output[1], dir);
    step_forward(dir, pos);
    // try to update mp
    visited.insert({pos, 0});
  }

  // find the size of colored grid
  const int x_min = min_element(begin(visited), end(visited), [](auto &lhs, auto &rhs) {
    return lhs.first.first < rhs.first.first;
  })->first.first;
  const int y_min = min_element(begin(visited), end(visited), [](auto &lhs, auto &rhs) {
    return lhs.first.second < rhs.first.second;
  })->first.second;
  const int x_max = max_element(begin(visited), end(visited), [](auto &lhs, auto &rhs) {
    return lhs.first.first < rhs.first.first;
  })->first.first;
  const int y_max = max_element(begin(visited), end(visited), [](auto &lhs, auto &rhs) {
    return lhs.first.second < rhs.first.second;
  })->first.second;
  // and initialize it with black
  vector<vector<char>> grid((y_max - y_min) + 1, vector<char>((x_max - x_min) + 1, ' '));

  // paint white cells
  for (auto &[pos, col] : visited) {
    if (col) {
      grid[pos.second - y_min][pos.first - x_min] = '#';
    }
  }

  // change Oy orientation
  reverse(begin(grid), end(grid));
  // and print the result
  for (auto &layer : grid) {
    for (auto &cell : layer)
      cout << cell;
    cout << endl;
  }

// My output:
// ###  ###    ##  ##  #### #### #  # ####
// #  # #  #    # #  # #       # # #  #
// #  # #  #    # #    ###    #  ##   ###
// ###  ###     # #    #     #   # #  #
// # #  #    #  # #  # #    #    # #  #
// #  # #     ##   ##  #    #### #  # #

#if dbg
  cin.close();
  cout << endl;
#endif
  return 0;
}