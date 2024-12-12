#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <set>
using namespace std;

vector<pair<int,int>> moves = {{-1,0}, {0,1}, {1,0}, {0,-1}};

vector<int> bfs(int startR, int startC, vector<vector<char>>& grid) {
    queue<pair<int,int>> q;
    q.push({startR, startC});
    char currLetter = grid[startR][startC];

    set<pair<int,int>> visited;

    while (!q.empty()) {
        pair<int,int> curr = q.front();
        q.pop();

        // do not visit again
        if (visited.find(curr) != visited.end()) continue;

        visited.insert({curr.first, curr.second});
        grid[curr.first][curr.second] = '-';

        // visit neighbours if not-visited & same letter
        int visitedN = 0;

        for (const auto& m : moves) {
            int nextR = curr.first + m.first;
            int nextC = curr.second + m.second;

            if (nextR < 0 || nextC < 0 || nextR >= grid.size() || nextC >= grid[0].size()) continue; // out of bounds

            if (visited.find({nextR, nextC}) != visited.end()) { // alr visited
                ++visitedN;
                continue;
            }
            if (currLetter != grid[nextR][nextC]) continue; // wrong letter

            q.push({nextR, nextC});
        }
    }

    int area = visited.size();
    int perimeter = 0;
    int sides = 0;

    for (const auto& p : visited) {

        // perimeter
        for (const auto & m : moves) {
            pair<int,int> currP = {p.first + m.first, p.second + m.second};
            if (visited.find(currP) == visited.end()) ++perimeter;
        }

        // outer corners
        if (visited.find({p.first - 1, p.second}) == visited.end() && visited.find({p.first, p.second-1}) == visited.end()) ++sides;
        if (visited.find({p.first + 1, p.second}) == visited.end() && visited.find({p.first, p.second-1}) == visited.end()) ++sides;
        if (visited.find({p.first - 1, p.second}) == visited.end() && visited.find({p.first, p.second+1}) == visited.end()) ++sides;
        if (visited.find({p.first + 1, p.second}) == visited.end() && visited.find({p.first, p.second+1}) == visited.end()) ++sides;

        // inner corners
        if (visited.find({p.first - 1, p.second}) != visited.end() && visited.find({p.first, p.second-1}) != visited.end() && visited.find({p.first - 1, p.second - 1}) == visited.end()) ++sides;
        if (visited.find({p.first + 1, p.second}) != visited.end() && visited.find({p.first, p.second-1}) != visited.end() && visited.find({p.first + 1, p.second - 1}) == visited.end()) ++sides;
        if (visited.find({p.first - 1, p.second}) != visited.end() && visited.find({p.first, p.second+1}) != visited.end() && visited.find({p.first - 1, p.second + 1}) == visited.end()) ++sides;
        if (visited.find({p.first + 1, p.second}) != visited.end() && visited.find({p.first, p.second+1}) != visited.end() && visited.find({p.first + 1, p.second + 1}) == visited.end()) ++sides;

    }

    // cout << "sides: " << sides << endl;

    return {area, perimeter, sides};
}

vector<vector<char>> parse() {
    vector<vector<char>> grid;

    string line;
    while (getline(cin, line)) {
        vector<char> row;
        stringstream ss(line);
        char ch;
        while (ss >> ch) row.emplace_back(ch);

        grid.emplace_back(row);
    }
    return grid;
}

void part(vector<vector<char>>& grid) {
    int cost1 = 0;
    int cost2 = 0;

    for (int i=0; i<grid.size(); ++i) {
        for (int j=0; j<grid[0].size(); ++j) {
            if (grid[i][j] != '-') {
                auto ret = bfs(i, j, grid);
                cost1 += ret[0] * ret[1];
                cost2 += ret[0] * ret[2];
            }
        }
    }

    cout << "part1: " << cost1 << endl << "part2: " << cost2 << endl;
}

int main() {
    vector<vector<char>> grid = parse();

    part(grid);
}
