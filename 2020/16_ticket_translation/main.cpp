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

using ticket_stats_t = vector<int>;
using required_ranges_t = vector<pair<int, int>>;

void parse_field_requirements(string_view line, unordered_map<string, required_ranges_t>& mp) {
    auto pos = line.find(':');
    if (pos == string::npos) {
        return;
    }
    string name = string(line.substr(0, pos));
    auto& ranges = mp[name];
    for (auto it = begin(line) + pos; it != end(line); ) {
        auto n_start = find_if(it, end(line), [](char ch) { return isdigit(ch); });
        if (n_start == end(line)) {
            break;
        }
        auto delim = find(n_start, end(line), '-');
        if (delim == end(line)) {
            break;
        }
        int lo = stoi(string(n_start, delim));
        auto space = find(delim, end(line), ' ');
        int hi = stoi(string(delim + 1, space));
        ranges.push_back({lo, hi});
        it = space;
    }
}

vector<int> parse_ticket_stats(string_view line) {
    vector<int> stats;
    for (auto it = begin(line); it != end(line); ) {
        auto delim = find(it, end(line), ',');
        stats.push_back(stoi(string(it, delim)));
        it = delim;
        if (it != end(line)) {
            it++;
        }
    }
    return stats;
}

void solve(istream &is, ostream &os) {
    unordered_map<string, required_ranges_t> field_ranges;
    vector<ticket_stats_t> ticket_stats;
    for (string line; getline(is, line); ) {
        if (line == "your ticket:") {
            is >> line;
            ticket_stats.push_back(parse_ticket_stats(line));
            break;
        }
        if (!line.empty()) {
            parse_field_requirements(line, field_ranges);
        }
    }

    for (string line; getline(is, line); ) {
        if (line.empty() || line == "nearby tickets:") {
            continue;
        }
        if (!line.empty()) {
            ticket_stats.push_back(parse_ticket_stats(line));
        }
    }

    vector<ticket_stats_t> valid_tickets;

    long long err_rate = 0;
    for (auto it = begin(ticket_stats); it != end(ticket_stats); it++) {
        bool is_valid_ticket = true;
        for (int v : *it) {
            bool is_valid_field = false;
            for (const auto& [field, ranges] : field_ranges) {
                for (const auto& range : ranges) {
                    if (v >= range.first && v <= range.second) {
                        is_valid_field = true;
                        break;
                    }
                }
            }
            if (!is_valid_field) {
                is_valid_ticket = false;
                err_rate += v;
            }
        }
        if (is_valid_ticket) {
            valid_tickets.push_back(*it);
        }
    }
    os << "A: " << err_rate << endl;

    const size_t positions = valid_tickets.front().size();
    // os << "valid tickets: " << positions << endl;

    vector<pair<int, unordered_set<string_view>>> availiable_fields(positions);
    for (size_t pos = 0; pos < positions; pos++) {
        availiable_fields[pos].first = pos;
        for (const auto& [field, ranges] : field_ranges) {
            bool is_valid_field = true;
            for (const auto& st : valid_tickets) {
                bool is_matched_ticket = false;
                for (const auto& range : ranges) {
                    int v = st[pos];
                    if (v >= range.first && v <= range.second) {
                        is_matched_ticket = true;
                        break;
                    }
                }
                if (!is_matched_ticket) {
                    is_valid_field = false;
                    break;
                }
            }
            if (is_valid_field) {
                availiable_fields[pos].second.insert(field);
            }
        }
    }
    sort(begin(availiable_fields), end(availiable_fields), [](auto& lhs, auto& rhs) {
        return lhs.second.size() < rhs.second.size();
    });
//    for (int pos = 0; pos < positions; pos++) {
//        os << availiable_fields[pos].first << " " << availiable_fields[pos].second.size() << endl;
//    }

    vector<size_t> departure_ids;
    for (int pos = 0; pos < positions; pos++) {
        assert(availiable_fields[pos].second.size() == 1);
        string_view v = *begin(availiable_fields[pos].second);
        if (v.find("departure") != string::npos) {
            departure_ids.push_back(availiable_fields[pos].first);
        }
        // os << availiable_fields[pos].first << " " << v << endl;
        for (int rest_pos = pos + 1; rest_pos < positions; rest_pos++) {
            availiable_fields[rest_pos].second.erase(v);
        }
    }

    long long result = 1;
    for (int id : departure_ids) {
        result *= valid_tickets[0][id];
    }
    os << "B: " << result << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
