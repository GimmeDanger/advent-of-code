#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <optional>
#include <array>
#include <cctype>
#include <cstring>
#include <sstream>
#include <unordered_set>
#include <map>

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

class field {
    string m_value;
    bool m_valid;
public:
    explicit field(const string &value, std::function<bool(const string&)> v)
            : m_value(value)
            , m_valid(v(m_value))
    {
    }
    bool is_valid() const {
        return m_valid;
    }
};

struct Passport {

    map<string, unique_ptr<field>> fields;
    const unordered_set<string> allowed_field_keys = {"byr", "iyr", "eyr", "hgt",
                                                      "hcl", "ecl", "pid"};


    void parse(string_view entry) {
        auto pos = entry.find(':');
        if (pos != string::npos) {
            string key = string(entry.substr(0, pos));
            string value = string(entry.substr(pos + 1));
            if (allowed_field_keys.count(key)) {
                std::function<bool(const string &)> validator;
                if (key == "byr") {
                    validator = [](const string &s) -> bool {
                        try {
                            if (s.size() != 4) {
                                return false;
                            }
                            if (find_if_not(s.begin(), s.end(), [](char ch) { return isdigit(ch); }) != s.end()) {
                                return false;
                            }
                            auto i = stoi(s);
                            return 1920 <= i && i <= 2002;
                        }
                        catch (...) {
                            return false;
                        }
                    };
                }
                else if (key == "iyr") {
                    validator = [](const string &s) -> bool {
                        try {
                            if (s.size() != 4) {
                                return false;
                            }
                            if (find_if_not(s.begin(), s.end(), [](char ch) { return isdigit(ch); }) != s.end()) {
                                return false;
                            }
                            auto i = stoi(s);
                            return 2010 <= i && i <= 2020;
                        }
                        catch (...) {
                            return false;
                        }
                    };
                }
                else if (key == "eyr") {
                    validator = [](const string &s) -> bool {
                        try {
                            if (s.size() != 4) {
                                return false;
                            }
                            if (find_if_not(s.begin(), s.end(), [](char ch) { return isdigit(ch); }) != s.end()) {
                                return false;
                            }
                            auto i = stoi(s);
                            return 2020 <= i && i <= 2030;
                        }
                        catch (...) {
                            return false;
                        }
                    };
                }
                else if (key == "hgt") {
                    validator = [](const string &s) -> bool {
                        try {
                            auto pos = s.find("cm");
                            if (pos != string::npos) {
                                auto i = stoi(s.substr(0, pos));
                                return pos + 2 == s.size() && 150 <= i && i <= 193;
                            }
                            else {
                                pos = s.find("in");
                                if (pos != string::npos) {
                                    auto i = stoi(s.substr(0, pos));
                                    return pos + 2 == s.size() && 59 <= i && i <= 76;
                                }
                                return false;
                            }
                        }
                        catch (...) {
                            return false;
                        }
                    };
                }
                else if (key == "hcl") {
                    validator = [](const string &s) -> bool {
                        try {
                            if (s.size() != 7) {
                                return false;
                            }
                            if (s[0] != '#') {
                                return false;
                            }
                            if (find_if_not(next(s.begin()), s.end(),
                                            [](char ch) { return isdigit(ch) || isalpha(ch); }) != s.end()) {
                                return false;
                            }
                            return true;
                        }
                        catch (...) {
                            return false;
                        }
                    };
                }
                else if (key == "ecl") {
                    validator = [](const string &s) -> bool {
                        return s == "amb" || s == "blu" ||
                               s == "brn" || s == "gry" ||
                               s == "grn" || s == "hzl" ||
                               s == "oth";
                    };
                }
                else if (key == "pid") {
                    validator = [](const string &s) -> bool {
                        try {
                            if (s.size() != 9) {
                                return false;
                            }
                            if (find_if_not(next(s.begin()), s.end(),
                                            [](char ch) { return isdigit(ch); }) != s.end()) {
                                return false;
                            }
                            return true;
                        }
                        catch (...) {
                            return false;
                        }
                    };
                }
                fields[key] = make_unique<field>(value, validator);
            }
        }
    }

    bool valid() const {
        for (auto& key : allowed_field_keys) {
            auto it = fields.find(key);
            if (it == fields.end() || !it->second->is_valid())
                return false;
        }
        return true;
    }

    void clear() {
        fields.clear();
    }
};

void solve(istream &is, ostream &os) {
    Passport pp; string entry; size_t count = 0;
    for (string line; getline(is, line); ) {
        if (line.empty()) {
            count += pp.valid();
            pp.clear();
        } else {
            stringstream ss(line);
            while (ss >> entry) {
                pp.parse(entry);
            }
        }
    }
    os << count << endl;
}

int main() {
    FAST_IO();
    INPUT_SOURCE();
    solve(cin, cout);
    return 0;
}
