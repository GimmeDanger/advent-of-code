#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <map>
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

vector<string> parseInput(istream& in) {
    string line;
    vector<string> input;
    while(getline(in, line)) {
        input.emplace_back(line);
    }
    return input;
}

struct Node {
    string name = "/";
    size_t size = 0;
    bool isDir = true;
    Node* parent = nullptr;
    map<string, unique_ptr<Node>> children;

    Node() = default;

    Node(string _name, size_t _size, bool _isDir, Node* _parent)
        : name(_name)
        , size(_size)
        , isDir(_isDir)
        , parent(_parent)
    {
    }

    void print() const {
        cout << "- " << name;
        if (isDir) {
            cout << " (dir)";
        }
        else {
            cout << " (file, size=" << size << ")";
        }
    }
};

class Filesystem {
    unique_ptr<Node> root;
    Node *curr = nullptr;

    static void printImpl(Node* root, int offset = 0) {
        if (!root) return;
        for (int i = 0; i < 2 * offset; i++)
            cout << " ";
        root->print();
        cout << endl;
        for (auto& [name, next] : root->children) {
            printImpl(next.get(), offset + 1);
        }
    }

    static void calcDirSizeImpl(Node* root, int size_limit, vector<long long>& totalSize) {
        if (!root) return;
        size_t fileSize = root->size;
        for (auto& [name, next] : root->children) {
            calcDirSizeImpl(next.get(), size_limit, totalSize);
            fileSize += next ? next->size : 0;
        }
        root->size = fileSize;
        if (root->isDir && fileSize < size_limit) {
            totalSize.push_back(fileSize);
        }
    }

public:
    Filesystem() = default;

    void addNodeToCurr(const string& typeOrSize, const string& name) {
        bool isDir = typeOrSize == "dir";
        curr->children[name] = make_unique<Node>(
                name,
                (isDir ? 0 : stoi(typeOrSize)),
                isDir,
                curr
        );
    }

    void updCurr(const string& path) {
        if (path == "/") {
            // init
            root = make_unique<Node>();
            curr = root.get();
        }
        else if (path == "..") {
            if (!curr || !curr->parent) return;
            curr = curr->parent;
        }
        else {
            if (!curr) return;
            auto it = curr->children.find(path);
            assert (it != curr->children.end());
            curr = it->second.get();
        }
    }

    void print() const {
        printImpl(root.get());
    }

    vector<long long> calcDirSizes(int size_limit) {
        vector<long long> totalSize;
        calcDirSizeImpl(root.get(), size_limit, totalSize);
        return totalSize;
    }
};


bool isCommand(const string& s) {
    return !s.empty() && s.front() == '$';
}

pair<string, optional<string>> parseCommand(const string& s) {
    auto slt = split(s, ' ');
    if (slt[1] == "ls") {
        return {"ls", nullopt};
    }
    return {"cd", slt[2]};
}

pair<string, string> parseFile(const string& s) {
    auto slt = split(s, ' ');
    return {slt[0], slt[1]};
}

// https://adventofcode.com/2022/day/7
Filesystem parseFs(const vector<string> &input, bool verbose = false) {
    cout << endl;
    Filesystem fs;
    for (const auto& line : input) {
        if (isCommand(line)) {
            auto cmd = parseCommand(line);
            if (cmd.first == "ls") continue;
            fs.updCurr(cmd.second.value());
        }
        else {
            auto file = parseFile(line);
            fs.addNodeToCurr(file.first, file.second);
        }
    }
    if (verbose)
        fs.print();
    return fs;
}

void solveA(const vector<string>& input) {
    cout << "A: ";
    auto fs = parseFs(input);
    auto res = fs.calcDirSizes(100000 /*size_limit*/);
    cout << accumulate(res.begin(), res.end(), 0ll) << endl;
}

void solveB(const vector<string>& input) {
    cout << "B: ";
    auto fs = parseFs(input);
    size_t total_size = 70000000;
    size_t req_free_space = 30000000;
    auto dirSizesSorted = fs.calcDirSizes(total_size /*size_limit*/);
    sort(dirSizesSorted.begin(), dirSizesSorted.end());
    size_t used_size = dirSizesSorted.back();
    for (long long n : dirSizesSorted) {
        if (used_size - n < total_size - req_free_space) {
            cout << n << endl;
            break;
        }

    }
}

int main() {
    ifstream in("input.txt");
    const auto input = parseInput(in);
    solveA(input);
    solveB(input);
    return 0;
}