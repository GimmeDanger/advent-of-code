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
#include <functional>

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

void solve(istream &is, ostream &os) {

    static constexpr int VALID_RANGE = 25;

    vector<long long> input = {0};
    for (long long i; is >> i; ) {
        input.push_back(i);
    }
    sort(begin(input), end(input));
    input.push_back(input.back() + 3);

    /**
        Find a chain that uses all of your adapters to connect the charging outlet to your
        device's built-in adapter and count the joltage differences between the charging outlet,
        the adapters, and your device. What is the number of 1-jolt differences multiplied by the
        number of 3-jolt differences?
    */
    array<size_t, 4> diff_cnt{};
    for (int i = 1; i < input.size(); i++) {
        diff_cnt[input[i] - input[i - 1]]++;
    }
    os << "A: " << diff_cnt[1] * diff_cnt[3] << endl;

    /**
        What is the total number of distinct ways you can arrange the adapters to connect
        the charging outlet to your device?
    */
    vector<long long> dp(input.size(), 0);
    dp[0] = 1;
    for (int i = 1; i < input.size(); i++) {
        for (int j = 1; j <= 3; j++) {
            if (i - j >= 0 && input[i] - input[i - j] <= 3) {
                dp[i] += dp[i - j];
            }
        }
    }
    os << "B: " << dp.back() << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
