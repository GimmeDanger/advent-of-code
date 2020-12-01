#include <bits/stdc++.h>
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

int two_sum(const vector<int> &input, const int target) {
    unordered_set<int> nums;
    for (int i : input) {
        auto it = nums.find(target - i);
        if (it != nums.end()) {
            return i * (*it);
        }
        nums.insert(i);
    }
    return -1;
}

long long three_sum(const vector<int> &input, const int target) {
    unordered_set<int> nums;
    for (int i = 0; i < input.size(); i++) {
        for (int j = i + 1; j < input.size(); j++) {
            auto it = nums.find(target - input[i] - input[j]);
            if (it != nums.end()) {
                return input[i] * 1ll * input[j] * (*it);
            }
        }
        nums.insert(input[i]);
    }
    return -1;
}

void solve(istream &is, ostream &os) {
    vector<int> input;
    for (int i = 0; is >> i; ) {
        input.push_back(i);
    }
    os << "A answer = " << two_sum(input, 2020) << endl;
    os << "B answer = " << three_sum(input, 2020) << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
