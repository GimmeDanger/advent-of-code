#include <bits/stdc++.h>
using namespace std;

#define FAST_IO()                                                              \
  ios::sync_with_stdio(false);                                                 \
  cout.tie(nullptr);                                                           \
  cin.tie(nullptr)

#define dbg 1

class Intcode {
  static constexpr int max_arg_num = 3;
  static constexpr int instr_offset = 1;
  static constexpr int special_value = numeric_limits<int>::max();

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
      if (EQUAL(instr, command::output) && output.size() == 3)
        break;
    }
    return output;
  }
};

class Screen {
 public:
  enum class tile_t {
    empty = 0, // No game object appears in this tile.
    wall,  // Walls are indestructible barriers.
    block, // Blocks can be broken by the ball.
    padle, // The paddle is indestructible.
    ball,  // The ball moves diagonally and bounces off objects.
    count
  };

 private:
  size_t width = 0;
  size_t height = 0;
  vector<vector<size_t>> data;

  static size_t tile_to_code(tile_t tile) {
    return static_cast<size_t>(tile);
  }

  static tile_t code_to_tile(size_t code) {
    assert(code >= 0 && code <= tile_to_code(tile_t::count));
    return static_cast<tile_t>(code);
  }

  static char tile_ascii(size_t code) {
    auto tile = code_to_tile(code);
    switch (tile) {
      case tile_t::empty:
        return ' ';
      case tile_t::wall:
        return 'H';
      case tile_t::block:
        return '$';
      case tile_t::padle:
        return '-';
      case tile_t::ball:
        return 'o';
      default:
        throw std::invalid_argument("Internal error: default tile ascii switch case");
    }
    return 0;
  }

 public:

  void show(ostream &ss) const {
    for (size_t i = 0; i < height; i++) {
      for (size_t j = 0; j < width; j++)
        ss << tile_ascii (data.at(i).at(j));
      ss << endl;
    }
  }

  size_t count(const tile_t tile) const {
    size_t cnt = 0;
    for (size_t i = 0; i < height; i++) {
      for (size_t j = 0; j < width; j++)
        if (code_to_tile (data.at(i).at(j)) == tile)
          cnt++;
    }
    return cnt;
  }

  void update(const int x, const int y, const size_t tile_code) {
    const bool y_updated = (y >= height);
    const bool x_updated = (x >= width);
    if (y_updated)
      height = y + 1;
    if (x_updated)
      width = x + 1;
    if (x_updated || y_updated) {
      data.resize(height, vector<size_t>(width, 0));
      for (auto &row : data) {
        if (x_updated)
          row.resize(width, 0);
      }
    }
    data.at(y).at(x) = tile_code;
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
  vector<long long> input;

  Screen screen;
  while (true) {
    auto out = cpu.eval(begin(input), end(input));
    if (cpu.is_halt())
      break;
    screen.update(out.at(0), out.at(1), out.at(2));
  }
  screen.show(cout);

  cout << endl << endl;
  cout << "Number of blocks: ";
  cout << screen.count(Screen::tile_t::block) << endl;

#if dbg
  cin.close();
  cout << endl;
#endif
  return 0;
}