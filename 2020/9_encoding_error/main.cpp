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

    vector<long long> input;
    for (long long i; is >> i; ) {
        input.push_back(i);
    }

    auto is_valid = [&input](int pos) {
        int first = max(pos - VALID_RANGE, 0);
        int last = max(pos - 1, 0);
        for (int i = first; i <= last; i++) {
            for (int j = i + 1; j <= last; j++) {
                if (input[i] + input[j] == input[pos]) {
                    return true;
                }
            }
        }
        return false;
    };

    /**
        The first step of attacking the weakness in the XMAS data is to find the first number
        in the list (after the preamble) which is not the sum of two of the 25 numbers before it.
        What is the first number that does not have this property?
    */
    int i = VALID_RANGE;
    while (i < input.size()) {
        if (!is_valid(i)) {
            break;
        }
        i++;
    }
    os << "A: " << (i < input.size() ? to_string(input[i]) : string("NO")) << endl;

    /**
        The final step in breaking the XMAS encryption relies on the invalid number you just
        found: you must find a contiguous set of at least two numbers in your list which sum
        to the invalid number from step 1. To find the encryption weakness, add together the
        smallest and largest number in this contiguous range.
        What is the encryption weakness in your XMAS-encrypted list of numbers?
    */

    int lo = 0, hi = -1;
    long long sum = 0;
    while (hi < i) {
        if (sum < input[i]) {
            hi++;
            sum += input[hi];
        } else if (sum > input[i]) {
            lo++;
            sum -= input[lo - 1];
        } else {
            break;
        }
    }
    long long lowest = *min_element(input.data() + lo, input.data() + hi + 1);
    long long greatest = *max_element(input.data() + lo, input.data() + hi + 1);
    os << "B: " << lowest + greatest << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
