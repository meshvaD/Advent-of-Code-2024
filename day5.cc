#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
using namespace std;

void parse(unordered_map<int, vector<int>>& rules, vector<vector<int>>& inputs) {
    // X | Y : X must be produced before Y (if produced)

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
}
// form map to quickly find index of any number in vec
unordered_map<int, int> inverseMapping(const vector<int>& vec) {
    unordered_map<int, int> index;
    for (int i=0; i<vec.size(); ++i)
        index[vec[i]] = i;
    return index;
}


bool checkValidOrdering(unordered_map<int, vector<int>>& rules, const vector<int>& vec) {
    unordered_map<int, int> index = inverseMapping(vec);

    // iter over vec starting from beginning & check rules
    for (int i=0; i<vec.size(); ++i) {
        vector<int> mustBefore = rules[vec[i]];
        for (const auto& n : mustBefore) {
            if (auto iter = index.find(n); iter != index.end() && iter->second < i) return false;
        }
    }
    return true;
}

bool makeValidOrdering(unordered_map<int, vector<int>>& rules, vector<int>& vec) {
    unordered_map<int, int> index = inverseMapping(vec);
    vector<int> proper;
    bool improper = false;

    for (int i=0; i<vec.size(); ++i) {
        vector<int> mustBefore = rules[vec[i]];
        int minIndex = i;

        for (const auto& n : mustBefore) {
            // cout << " b: " << n <<  " index: " << index[n] << endl;
            if (auto iter = index.find(n); iter != index.end() && iter->second < index[vec[i]]) {
                minIndex = min(minIndex, iter->second);
            }
        }

        if (minIndex != i) improper = true;

        // insert elem at minIndex & shift index values of all rest
        proper.insert(proper.begin() + minIndex, vec[i]);
        for (int j=minIndex; j<i; ++j)
            index[vec[j]] = j + 1;
    }

    vec = proper;

    return improper;
}

int part1(unordered_map<int, vector<int>>& rules, const vector<vector<int>>& inputs) {
    int out = 0;
    for (const auto& vec : inputs) {
        bool succ = checkValidOrdering(rules, vec);
        if (succ) {
            int mid = vec[vec.size()/2];
            out += mid;
        }
    }
    return out;
}

int part2(unordered_map<int, vector<int>>& rules, vector<vector<int>>& inputs) {
    int out = 0;
    for (vector<int>& vec : inputs) {
        bool improper = makeValidOrdering(rules, vec);
        bool updated = false;
        while (improper) {
            improper = makeValidOrdering(rules, vec);
            updated = true;
        }
        if (updated) {
            int mid = vec[vec.size()/2];
            out += mid;
        }
    }
    return out;
}

int main() {
    unordered_map<int, vector<int>> rules;
    vector<vector<int>> inputs;

    parse(rules, inputs);
    
    cout << "part1: " << part1(rules, inputs) << endl;
    cout << "part2: " << part2(rules, inputs) << endl;
}
