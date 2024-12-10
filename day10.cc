#include <vector>
#include <iostream>
#include <queue>
#include <set>
#include <map>
#include <sstream>
using namespace std;

vector<pair<int,int>> delta = {{0,1}, {0,-1}, {1,0}, {-1,0}};

int doBFS(int startR, int startC, const vector<vector<int>>& grid) {
    int count = 0;

    set<pair<int,int>> visited;
    queue<pair<int,int>> bfs;

    bfs.push({startR, startC});
    visited.insert({startR, startC});

    while (!bfs.empty()) {
        auto top = bfs.front();
        bfs.pop();

        if (grid[top.first][top.second] == 9) {
            // cout << "got to 9 at: " << top.first << " , " << top.second << endl;
            ++count;
            continue;
        }
        
        // add neighbours
        for (const auto & d : delta) {
            int nextR = top.first + d.first;
            int nextC = top.second + d.second;

            if (nextR >= 0 && nextC >= 0 && nextR < grid.size() && nextC < grid[0].size() 
                && visited.find({nextR, nextC}) == visited.end() && grid[nextR][nextC] == grid[top.first][top.second]+1) {
                    visited.insert({nextR, nextC});
                    bfs.push({nextR, nextC});
                } 
        }
    }
    return count;
}

int doBFSCountAllPaths(int startR, int startC, const vector<vector<int>>& grid) {
    int count = 0;

    map<pair<int,int>, int> visitedCount;
    queue<pair<int,int>> bfs;

    bfs.push({startR, startC});
    visitedCount[{startR, startC}] = 1;

    while (!bfs.empty()) {
        auto top = bfs.front();
        bfs.pop();

        // cout << "visiting: " << top.first << " , " << top.second << " : " << grid[top.first][top.second] << " count: " << visitedCount[top] << endl;

        if (grid[top.first][top.second] == 9) {
            // cout << "got to 9 at: " << top.first << " , " << top.second << endl;
            count += visitedCount[top];
            continue;
        }
        
        // add neighbours
        for (const auto & d : delta) {
            int nextR = top.first + d.first;
            int nextC = top.second + d.second;

            if (nextR >= 0 && nextC >= 0 && nextR < grid.size() && nextC < grid[0].size() && grid[nextR][nextC] == grid[top.first][top.second]+1) {
                if (auto it = visitedCount.find({nextR, nextC}); it != visitedCount.end()) {
                    it->second += visitedCount[top];
                } else {
                    visitedCount[{nextR, nextC}] = visitedCount[top];
                    bfs.push({nextR, nextC});
                }
            }
        }
    }
    return count;
}

int part1(const vector<vector<int>>& grid) {
    int score = 0;
    for (int r=0; r<grid.size(); ++r) {
        for (int c=0; c<grid[0].size(); ++c) {
            // if trailhead, get score
            if (grid[r][c] == 0) 
                score += doBFS(r, c, grid);
        }
    }
    return score;
}

int part2(const vector<vector<int>>& grid) {
    int score = 0;
    for (int r=0; r<grid.size(); ++r) {
        for (int c=0; c<grid[0].size(); ++c) {
            // if trailhead, get score
            if (grid[r][c] == 0) 
                score += doBFSCountAllPaths(r, c, grid);
        }
    }
    return score;
}

vector<vector<int>> parse() {
    vector<vector<int>> grid;

    string row;
    while (cin >> row) {
        vector<int> rowVec;

        stringstream ss(row);
        char c;
        while (ss >> c) rowVec.emplace_back(c - '0');

        grid.emplace_back(rowVec);
    }
    return grid;
}

int main() {
    vector<vector<int>> grid = parse();

    int p1 = part1(grid);
    cout << "part1: " << p1 << endl;

    int p2 = part2(grid);
    cout << "part2: " << p2 << endl;
}
