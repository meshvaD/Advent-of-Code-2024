#include <iostream>
#include <vector>
#include <sstream>
#include <queue>
#include <unordered_set>
#include <unordered_map>
using namespace std;

vector<vector<char>> parse(int& startR, int& startC, int& endR, int& endC) {
    vector<vector<char>> grid;

    string line;
    while (cin >> line) {
        stringstream ss(line);
        vector<char> row;
        char c;
        while (ss >> c) {
            if (c == 'S') {
                startR = grid.size();
                startC = row.size();
                row.emplace_back('.');
            } else if (c == 'E') {
                endR = grid.size();
                endC = row.size();
                row.emplace_back('.');
            } else {
                row.emplace_back(c);
            }
        }
        grid.emplace_back(row);
    }
    return grid;
}

enum Dir {UP, RIGHT, DOWN, LEFT};
vector<pair<int,int>> moves = {{-1,0},{0,1},{1,0},{0,-1}};

struct Pos {
    int r, c;
    Dir dir;
    int cost = 0;
    // Pos(int r, int c, Dir dir, int cost) : r(r), c(c), dir(dir), cost(cost) {}
};

// take one step in same dir/ turn around - returns new pos
std::vector<Pos> advance(const Pos& curr) {
    std::vector<Pos> nextPos;

    // in same dir
    nextPos.emplace_back(Pos{curr.r + moves[curr.dir].first, curr.c + moves[curr.dir].second, curr.dir, curr.cost+1});

    // make a turn
    nextPos.emplace_back(Pos{curr.r, curr.c, (Dir)((curr.dir + 1) % 4), curr.cost + 1000});
    nextPos.emplace_back(Pos{curr.r, curr.c, (Dir)((curr.dir - 1 + 4) % 4), curr.cost + 1000});

    return nextPos;
}

bool operator<(const Pos& a, const Pos& b) {
    return a.cost > b.cost;
}

bool operator==(const Pos& a, const Pos& b) {
    return (a.r == b.r) && (a.c == b.c) && (a.dir == b.dir);
}

struct hash_Pos {
    size_t operator()(const Pos& p) const
    {
        return (p.r << 16) | (p.c << 2) | p.dir;
    }
};

struct hash_Pair {
    size_t operator()(const pair<int,int>& p) const
    {
        return (p.first << 16) | p.second;
    }
};

int part1(const vector<vector<char>>& grid, int startR, int startC, int endR, int endC) {
    Pos start{startR, startC, Dir::RIGHT, 0};

    priority_queue<Pos> q;
    q.push(start);
    
    unordered_set<Pos, hash_Pos> visited;
    visited.insert(start);

    while (!q.empty()) {
        Pos curr = q.top(); q.pop();

        // check if end reached
        if (curr.r == endR && curr.c == endC) {
            // best path reached
            return curr.cost;
        }

        for (Pos& p : advance(curr)) {
            // check this is moveable & not alr visited
            if (grid[p.r][p.c] != '.' || visited.find(p) != visited.end()) continue;
            q.push(p); visited.insert(curr);
        }
    }
}

int part2(const vector<vector<char>>& grid, int startR, int startC, int endR, int endC) {
    Pos start{startR, startC, Dir::RIGHT, 0};

    priority_queue<Pos> q;
    q.push(start);
    
    unordered_set<Pos, hash_Pos> visited;
    visited.insert(start);

    unordered_map<Pos, unordered_set<Pos, hash_Pos>, hash_Pos> previousPaths;
    vector<Pos> endPoses; // can have multiple "best paths"

    int bestCost = -1;

    while (!q.empty()) {
        Pos curr = q.top(); q.pop();
        // visited.insert(curr);

        // cout << "find visit: " << curr.r << " , " << curr.c << " dir: " << curr.dir << " cost: " << curr.cost << endl;

        // exceeded best score - this path is redundant
        if (bestCost >= 0 && curr.cost > bestCost)
            continue;

        // check if end reached
        if (curr.r == endR && curr.c == endC) {
            // best path reached
            endPoses.emplace_back(curr);
            bestCost = curr.cost;
            // break;
        }

        for (Pos& p : advance(curr)) {
            // check this is moveable
            if (grid[p.r][p.c] != '.') continue;

            // cout << "\tadd: " << p.r << " , " << p.c << " dir: " << p.dir << " cost: " << p.cost << endl;

            if (auto iter = visited.find(p); iter != visited.end()) {
                if (iter->cost < p.cost) 
                    continue;
                else if (iter->cost == p.cost) {
                    previousPaths[p].insert(curr); // add current neighbour
                    // cout << "\t\ta" << endl;
                } else {
                    previousPaths[p] = {curr}; // replace current
                    // cout << "\t\tb" << endl;
                }
            } else {
                // cout << "\t\tc" << endl;
                visited.insert(p);
                previousPaths[p] = {curr};
            }
            q.push(p);
        }
    }

    // backtrack

    queue<Pos> backtrack;
    for (auto& p : endPoses)
        backtrack.push(p);

    int count = 0;
    unordered_set<pair<int,int>, hash_Pair> bt_visited;

    while (!backtrack.empty()) {
        Pos curr = backtrack.front(); backtrack.pop();
        pair<int,int> coord {curr.r, curr.c};

        bt_visited.insert(coord);
        ++count;

        for (auto& neighbour : previousPaths[curr]) {
            backtrack.push(neighbour);
        }
    }
    return bt_visited.size();
}

int main() {
    int startR, startC, endR, endC;
    vector<vector<char>> grid = parse(startR, startC, endR, endC);

    int p1 = part1(grid, startR, startC, endR, endC);
    cout << "part1: " << p1 << endl;

    int p2 = part2(grid, startR, startC, endR, endC);
    cout << "part2: " << p2 << endl;
}
