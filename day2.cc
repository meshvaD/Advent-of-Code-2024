#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
using namespace std;

bool failedBounds(int diff, bool incr) {
    return (incr && (diff < 1 || diff > 3)) || (!incr && (-diff < 1 || -diff > 3));
}

bool checkSafety(vector<int> vec) {
    bool incr = vec[1] > vec[0];

    for (int i=1; i<vec.size(); ++i) {
        int diff = vec[i] - vec[i-1];

        if (failedBounds(diff, incr)) return false;
    }
    return true;
}

int part1(const vector<vector<int>>& input) {
    int count = 0;

    for (const vector<int> v : input) {
        if (checkSafety(v)) ++count;
    }
    return count;
}

int part2(const vector<vector<int>>& input) {
    int count = 0;

    for (const vector<int> v : input) {
        // this is bruteforce-y :( - BUT the input is very small, v is max ~10 items
        for (int i=0; i<v.size(); ++i) {
            vector<int> rem = v;
            rem.erase(rem.begin() + i);
            if (checkSafety(rem)) {
                ++count;
                break;
            }
        }
    }
    return count;
}

int main() {
    
    std::ifstream infile("input/2.txt");
    vector<vector<int>> input;

    string line;
    while (getline(infile, line)) {
        stringstream s(line);
        vector<int> seq;

        int n;
        while (s >> n) seq.emplace_back(n);
        input.emplace_back(seq);
    }

    cout << "part1: " << part1(input) << endl;
    cout << "part2: " << part2(input) << endl;
}
