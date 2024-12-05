#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <algorithm>
using namespace std;

class Ordering {
    static unordered_map<int, vector<int>> rules;
    static vector<vector<int>> inputs;
public:
    struct sortWithRules
    {
        inline bool operator() (const int a, const int b)
        {
            auto aRules = rules[a];
            auto iter = find(aRules.begin(), aRules.end(), b); // determine if a must be before b
            return iter != aRules.end();
        }
    };

    static int part2() {
        int out = 0;
        for (vector<int>& vec : inputs) {
            bool isSorted = is_sorted(vec.begin(), vec.end(), sortWithRules());
            if (!isSorted) {
                sort(vec.begin(), vec.end(), sortWithRules());
                int mid = vec[vec.size()/2];
                out += mid;
            }
        }
        return out;
    }

    static int part1() {
        int out = 0;
        for (const auto& vec : inputs) {
            bool succ = is_sorted(vec.begin(), vec.end(), sortWithRules());
            if (succ) {
                int mid = vec[vec.size()/2];
                out += mid;
            }
        }
        return out;
    }
};

unordered_map<int, vector<int>> parseRules() {
    // X | Y : X must be produced before Y (if produced)
    unordered_map<int, vector<int>> rules;

    string s;
    while (getline(cin, s)) {
        if (s.size() == 0) break;

        stringstream ss(s);
        int before, after;
        ss >> before;
        char temp; ss >> temp;
        ss >> after;

        if (auto iter = rules.find(before); iter != rules.end()) {
            iter->second.emplace_back(after);
        } else {
            rules[before] = {after};
        }
    }
    return rules;
}

vector<vector<int>> parseInputs() {
    string s;
    vector<vector<int>> inputs;
    while (getline(cin, s)) {
        stringstream ss(s);

        vector<int> curr;
        int v;
        char temp;
        while (ss >> v) {
            curr.emplace_back(v);
            ss >> temp;
        }
        inputs.emplace_back(curr);
    }
    return inputs;
}

unordered_map<int, vector<int>> Ordering::rules = parseRules();
vector<vector<int>> Ordering::inputs = parseInputs();

int main() {
    Ordering o;
    cout << "part1: " << Ordering::part1() << endl;
    cout << "part2: " << Ordering::part2() << endl;
}
