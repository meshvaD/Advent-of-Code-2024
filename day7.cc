#include <iostream>
#include <vector>
#include <queue>
#include <sstream>
#include <math.h>
using namespace std;

struct Info {
    u_int64_t target;
    vector<u_int64_t> vals;
};

bool checkPossible(const vector<u_int64_t>& vec, u_int64_t target, bool doConcatOp) {
    queue<u_int64_t> possibleVals;

    possibleVals.push(vec[0]);
    for (int i=1; i<vec.size(); ++i) {
        // push values if you used + or * or || on prev vals
        int prevSize = possibleVals.size();
        for (int j=0; j<prevSize; ++j) {
            u_int64_t val = possibleVals.front(); possibleVals.pop();
            if (val > target) continue;
            possibleVals.push(val * vec[i]);
            possibleVals.push(val + vec[i]);

            if (doConcatOp) {
                u_int64_t k = val * pow(10, int(log10(vec[i]))+1) + vec[i];
                possibleVals.push(k);
            }
        }
    }

    // final stack - check if any possible vals match the target
    while (!possibleVals.empty()) {
        u_int64_t val = possibleVals.front();
        possibleVals.pop();
        if (val == target) return true;
    }
    return false;
}

vector<Info> parse() {
    vector<Info> info;

    string s;
    while (getline(cin, s)) {
        stringstream ss(s);

        u_int64_t target;
        ss >> target;

        char temp; ss>>temp; // read semi colon

        vector<u_int64_t> vals;
        u_int64_t v;
        while (ss >> v) vals.emplace_back(v);

        info.emplace_back(Info{target, vals});
    }
    return info;
}

u_int64_t part(const vector<Info>& info, bool isPart1) {
    u_int64_t ret = 0;

    for (int i=0; i<info.size(); ++i) {
        bool succ = checkPossible(info[i].vals, info[i].target, !isPart1);
        if (succ) ret += info[i].target;
    }
    return ret;
}

int main() {
    vector<Info> info = parse();
    u_int64_t p1 = part(info, true);
    u_int64_t p2 = part(info, false);

    cout << "part1: " << p1 << endl;
    cout << "part2: " << p2 << endl;
}
