#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <queue>
#include <unordered_set>
#include <unordered_map>
using namespace std;

vector<string> split(string s, string delimiter) {
    size_t pos_start = 0, pos_end;
    string token;
    vector<string> res;
    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delimiter.size();
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}

vector<string> parseInput(istream& in) {
    string line;
    vector<string> input;
    while(getline(in, line)) {
        input.emplace_back(line);
    }
    return input;
}

// {sensor_coord, closest_beacon_coord}
pair<pair<long long, long long>, pair<long long, long long>> parseLine(const string& line) {
    // Format:
    // Sensor at x=20, y=1: closest beacon is at x=15, y=3
    pair<long long, long long> sensor, beacon;
    auto xt = find(line.begin(), line.end(), 'x');
    auto yt = find(next(xt), line.end(), 'y');
    sensor.first = stoi(string(next(next(xt)), find(xt, line.end(), ',')));
    sensor.second = stoi(string(next(next(yt)), find(yt, line.end(), ':')));
    xt = find(yt, line.end(), 'x');
    yt = find(next(xt), line.end(), 'y');
    beacon.first = stoi(string(next(next(xt)), find(xt, line.end(), ',')));
    beacon.second = stoi(string(next(next(yt)), find(yt, line.end(), ':')));
    return {sensor, beacon};
}

long long c_min = numeric_limits<long long>::max();
long long c_max = numeric_limits<long long>::min();
vector<pair<long long, long long>> sensors;
vector<pair<long long, long long>> beacons;


void parseInput(const vector<string>& input) {
    for (const auto& line : input) {
        auto [s, b] = parseLine(line);
        auto diff = abs(s.second - b.second) + abs(s.first - b.first);
        c_min = min(c_min, s.first - diff);
        c_max = max(c_max, s.first + diff);
        sensors.push_back(s);
        beacons.push_back(b);
    }
}

long long solve(long long r, long long lo, long long hi, bool verbose = false) {
    unsigned long long row_cnt = 0;
    for (long long c = max(lo, c_min); c != min(hi, c_max); c++) {
        bool isEmpty = true;
        for (int i = 0; i < sensors.size(); i++) {
            if (r == sensors[i].second && c == sensors[i].first) {
                isEmpty = false;
                break;
            }
            if (r == beacons[i].second && c == beacons[i].first) {
                isEmpty = false;
                break;
            }
        }
        if (!isEmpty) {
            continue;
        }
        bool nearSensor = false;
        for (int i = 0; i < sensors.size(); i++) {
            auto diff = abs(sensors[i].second - beacons[i].second) + abs(sensors[i].first - beacons[i].first);
            if (abs(sensors[i].second - r) + abs(sensors[i].first - c) <= diff) {
                nearSensor = true;
                row_cnt++;
                break;
            }
        }
        if (!nearSensor && verbose) {
            // beacon candidate
            cout << r << " " << c << endl;
        }
    }
    return row_cnt;
}

constexpr long long R = 2000000;
constexpr long long LO = 0;
constexpr long long HI = 4000000;

void solveA(const vector<string>& input, bool verbose = false) {
    cout << "A: " << endl;
    parseInput(input);
    cout << solve(R, c_min, c_max) << endl;
}

void solveB(const vector<string>& input) {
    cout << "B: " << endl;
    parseInput(input);
    for (int r = LO; r <= HI; r++) {
        // if (r % 10000 == 0)
        {
            cout << r << " row" << endl;
        }
        solve(r, LO, HI, true);
    }
}

int main() {
    ifstream in("input.txt");
    const auto input = parseInput(in);
    solveA(input);
    solveB(input);
    return 0;
}