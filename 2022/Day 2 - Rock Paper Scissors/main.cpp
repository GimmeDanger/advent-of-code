#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
using namespace std;

vector<string> split(string s, char delimiter) {
    size_t pos_start = 0, pos_end;
    string token;
    vector<string> res;
    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + 1;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}

vector<string> parse(istream& in) {
    string line;
    vector<string> input;
    while(getline(in, line)) {
        input.emplace_back(line);
    }
    return input;
}

int roundResult(int opponent, int player) {
    // Rock defeats Scissors, Scissors defeats Paper, and Paper defeats Rock
    // Rock = 1, Paper = 2, Scissors = 3
    if (opponent == player)
        return 0; //< draw if simular
    else if ((player == 1 && opponent == 3) || (player == 2 && opponent == 1) || (player == 3 && opponent == 2))
        return 1; //< win
    else
        return -1; //< loose
}

int roundScore(int opponent, int player) {
    int result = roundResult(opponent, player);
    int resultScore = result >= 0 ? result * 3 + 3 : 0;
    return player + resultScore;
}

// result start from 1
int opponentMove(char ch) {
    assert(ch == 'A' || ch == 'B' || ch == 'C');
    return ch - 'A' + 1;
}

// result start from 1
int playerMove(char ch, int opp_move, int strategy) {
    assert(ch == 'X' || ch == 'Y' || ch == 'Z');
    int parsed = ch - 'X' + 1;
    if (strategy == 0) {
        return parsed; //< player_move
    }
    else {
        int outcome = (parsed == 1 ? -1 : parsed == 2 ? 0 : 1);
        for (int pl_move = 1; pl_move <= 3; pl_move++) {
            if (roundResult(opp_move, pl_move) == outcome)
                return pl_move;
        }
        assert(0);
        return -1;
    }
}

// https://adventofcode.com/2022/day/2
int solve(const vector<string> &input, int strategy) {
    long long score = 0;
    for (auto& line : input) {
        int opp_move = opponentMove(line[0]);
        int pl_move = playerMove(line[2], opp_move, strategy);
        score += roundScore(opp_move, pl_move);
    }
    return score;
}

void solve_A(const vector<string>& input) {
    cout << "A: ";
    cout << solve(input, 0 /*strategy*/) << endl;
}

void solve_B(const vector<string>& input) {
    cout << "B: ";
    cout << solve(input, 1 /*strategy*/) << endl;
}

int main() {
    ifstream in("input.txt");
    const auto input = parse(in);
    solve_A(input);
    solve_B(input);
    return 0;
}