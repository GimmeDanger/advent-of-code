#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <optional>
#include <array>
#include <cctype>
#include <cstring>
#include <string>
#include <string_view>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <charconv>
#include <algorithm>

using namespace std;

#define dbg 1

#if dbg
#define INPUT_SOURCE() ifstream cin("input.txt")
#else
#define INPUT_SOURCE()
#endif

#define FAST_IO()              \
  ios::sync_with_stdio(false); \
  cout.tie(nullptr);           \
  cin.tie(nullptr)

enum class command_t {
    nop,
    acc,
    jmp,
    err
};
using command = pair<command_t, int>;

void solve(istream &is, ostream &os) {

    vector<command> boot_code;
    for (string cmd; is >> cmd;) {
        command next;
        next.first = cmd == "nop" ? command_t::nop
                                  : cmd == "acc" ? command_t::acc
                                                 : cmd == "jmp" ? command_t::jmp
                                                                : command_t::err;
        is >> next.second;
        boot_code.push_back(std::move(next));
    }

    unordered_set<int> used;
    auto run_code = [&used, &boot_code](int i, int &loop_start) -> int {
        used.clear();
        int acc_value = 0;
        while (i < boot_code.size() && used.count(i) == 0) {
            used.insert(i);
            auto &curr = boot_code[i];
            if (curr.first == command_t::acc) {
                acc_value += curr.second;
                i++;
            } else if (curr.first == command_t::jmp) {
                i += curr.second;
            } else if (curr.first == command_t::nop) {
                i++;
            }
        }
        loop_start = (i < boot_code.size() ? i : -1);
        return acc_value;
    };

    int loop_start = -1;
    os << "A: " << run_code(0, loop_start) << endl;

    for (int i = 0, acc_value = 0; i < boot_code.size();) {
        auto &curr = boot_code[i];
        if (curr.first == command_t::acc) {
            acc_value += curr.second;
            i++;
        } else {
            // swap command
            if (curr.first == command_t::nop) curr.first = command_t::jmp;
            else if (curr.first == command_t::jmp) curr.first = command_t::nop;

            auto res = run_code(i, loop_start);

            // swap command
            if (curr.first == command_t::nop) curr.first = command_t::jmp;
            else if (curr.first == command_t::jmp) curr.first = command_t::nop;

            if (loop_start < 0) {
                acc_value += res;
                os << "B: " << acc_value << endl;
                break;
            } else if (curr.first == command_t::jmp) {
                i += curr.second;
            } else if (curr.first == command_t::nop) {
                i++;
            }
        }
    }
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
