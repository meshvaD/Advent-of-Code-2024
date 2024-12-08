#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
using namespace std;

struct hash_pair {
    size_t operator()(const pair<int,int>& p) const
    {
        return (p.first << 16) | p.second;
    }
};

bool inBounds(int x, int y, int maxR, int maxC) {
    if (x >=0 && y >= 0 && x < maxR && y < maxC) return true;
    return false;
}

void findAntinodesForType(const vector<pair<int,int>>& pos, unordered_set<pair<int,int>, hash_pair>& visited, int maxR, int maxC, bool part1) {
    // look for every pair in the vector & determine antinodes
    for (int i=0; i<pos.size(); ++i) {
        for (int j=i+1; j<pos.size(); ++j) {

            // dist between pair
            double x_dist = pos[i].first - pos[j].first;
            double y_dist = pos[i].second - pos[j].second;

            // extend from that line to find antinodes
            int mult = part1 ? 1 : 0; // only include the current antennae if in part2

            while (inBounds(pos[i].first + mult * x_dist, pos[i].second + mult * y_dist, maxR, maxC)) { // consider the entire line in this dir, until off the grid
                visited.insert({pos[i].first + mult * x_dist, pos[i].second + mult * y_dist});
                ++mult;
                if (part1) break;
            }

            mult = part1 ? 1 : 0; // only include the paired anteannae if in part2

            while (inBounds(pos[j].first - mult * x_dist, pos[j].second - mult * y_dist, maxR, maxC)) {
                visited.insert({pos[j].first - mult * x_dist, pos[j].second - mult * y_dist});
                ++mult;
                if (part1) break;
            }

        }
    }
}

unordered_map<char, vector<pair<int,int>>> parse(int& maxR, int& maxC) {
    unordered_map<char, vector<pair<int,int>>> allCoords;
    string s;

    int r = -1; int c = -1;

    while (getline(cin, s)) {
        ++r; 
        maxR = max(r+1, maxR);
        stringstream ss(s);
        char ch;
        while (ss >> ch) {
            c++; 
            maxC = max(c+1, maxC);
            if (ch == '.') continue;
            pair<int,int> curr {r,c};

            if (auto iter = allCoords.find(ch); iter != allCoords.end())
                iter->second.emplace_back(curr);
            else
                allCoords[ch] = {curr};
        }
        c = -1;
    }
    return allCoords;
}

int part(unordered_map<char, vector<pair<int,int>>>& allCoords, int maxR, int maxC, bool part1) {
    unordered_set<pair<int,int>, hash_pair> visited;

    for (auto coords : allCoords) {
        findAntinodesForType(coords.second, visited, maxR, maxC, part1);
    }
    return visited.size();
}

int main() {
    int maxR = -1; int maxC = -1;
    unordered_map<char, vector<pair<int,int>>> allCoords = parse(maxR, maxC);

    cout << "part1: " << part(allCoords, maxR, maxC, true) << endl;
    cout << "part2: " << part(allCoords, maxR, maxC, false) << endl;
}
