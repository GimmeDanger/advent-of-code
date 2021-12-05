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

vector<int> split (string s, char delimiter) {
  size_t pos_start = 0, pos_end;
  string token;
  vector<int> res;
  while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
    token = s.substr (pos_start, pos_end - pos_start);
    pos_start = pos_end + 1;
    res.push_back (stoi(token));
  }
  res.push_back (stoi(s.substr (pos_start)));
  return res;
}

constexpr int BOARD_SIDE = 5;
using Board = vector<vector<int>>;

int step(Board& b, int t) {
  // add number
  bool updated = false;
  for (int i = 0; i < BOARD_SIDE && !updated; i++) {
    for (int j = 0; j < BOARD_SIDE && !updated; j++) {
      if (b[i][j] == t + 1) {
        // started from 1, so marked could be stored as negative
        b[i][j] *= -1;
        updated = true;
      }
    }
  }
  // check winning rows
  bool row_win = false;
  for (int i = 0; i < BOARD_SIDE && !row_win; i++) {
    bool all_selected = true;
    for (int j = 0; j < BOARD_SIDE && !row_win; j++) {
      if (b[i][j] > 0) {
        all_selected = false;
        break;
      }
    }
    if (all_selected) {
      row_win = true;
      break;
    }
  }
  // check winning col
  bool col_win = row_win;
  for (int j = 0; j < BOARD_SIDE && !col_win; j++) {
    bool all_selected = true;
    for (int i = 0; i < BOARD_SIDE && !col_win; i++) {
      if (b[i][j] > 0) {
        all_selected = false;
        break;
      }
    }
    if (all_selected) {
      col_win = true;
      break;
    }
  }
  // calc winning board score
  int score = -1;
  if (col_win || row_win) {
    score = 0;
    for (int i = 0; i < BOARD_SIDE; i++) {
      for (int j = 0; j < BOARD_SIDE; j++) {
        if (b[i][j] > 0) {
          score += (b[i][j] - 1);
        }
      }
    }
    score *= t;
  }
  return score;
}

void parse_input(const vector<string>& input, vector<int>& turns, vector<Board>& boards, unordered_map<int, unordered_set<int>>& num_to_board_id) {
  // parse first line of input, bingo turns
  turns = split(input[0], ',');

  // parse boards
  boards.clear();
  num_to_board_id.clear();
  for (size_t i = 2; i < input.size(); i++) {
    boards.push_back(vector<vector<int>>(BOARD_SIDE, vector<int>(BOARD_SIDE, 0)));
    for (int j = 0; j < BOARD_SIDE; j++) {
      stringstream ss(input[i + j]);
      for (int k = 0, n = 0; k < BOARD_SIDE; k++) {
        ss >> n;
        num_to_board_id[n].insert(boards.size() - 1);
        n++; //< should start from zero to store marked as negative
        boards.back()[j][k] = n;
      }
    }
    i += BOARD_SIDE;
  }
}

void solve_A(const vector<string>& input) {
  vector<int> turns;
  vector<Board> boards;
  unordered_map<int, unordered_set<int>> num_to_board_id;
  parse_input(input, turns, boards, num_to_board_id);
  const int n_boards = (input.size() - 1) / (BOARD_SIDE + 1);
  assert(boards.size() == (size_t) n_boards);

  // solution
  int res = [&turns, &boards, &num_to_board_id]() {
    for (int t : turns) {
      for (int b_i : num_to_board_id[t]) {
        int score = step(boards[b_i], t);
        if (score >= 0) {
          return score;
        }
      }
    }
    return -1;
  }();
  cout << "A: ";
  cout << res << endl;
}

void solve_B(const vector<string>& input) {
  vector<int> turns;
  vector<Board> boards;
  unordered_map<int, unordered_set<int>> num_to_board_id;
  parse_input(input, turns, boards, num_to_board_id);
  const int n_boards = (input.size() - 1) / (BOARD_SIDE + 1);
  assert(boards.size() == (size_t) n_boards);

  // solution
  int res = [&turns, &boards, &num_to_board_id]() -> int {
    int n_boards = boards.size();
    unordered_set<int> finished_board_id;
    for (int t : turns) {
      auto& possible_boards = num_to_board_id[t];
      for (auto it = begin(possible_boards); it != end(possible_boards); ) {
        if (finished_board_id.count(*it))
          it = possible_boards.erase(it);
        else
          it++;
      }
      for (auto it = begin(possible_boards); it != end(possible_boards); it++) {
        int score = step(boards[*it], t);
        if (score >= 0) {
          if (n_boards == 1) {
            return score;
          } else {
            finished_board_id.insert(*it);
            n_boards--;
          }
        }
      }
    }
    return -1;
  }();
  cout << "B: ";
  cout << res << endl;
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