#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <set>
using namespace std;

vector<vector<char>> parseGrid(int& startR, int& startC) {
    vector<vector<char>> grid;
    string line;

    while (getline(cin, line)) {
        if (line.size() <= 1) break;

        stringstream ss(line);
        char c;
        vector<char> row;
        while (ss >> c) {
            if (c == '@') {
                startR = grid.size();
                startC = row.size();
                c = '.';
            }
            row.emplace_back(c);
        }

        grid.emplace_back(row);
    }
    return grid;
}

string parseMoves() {
    string moves = "";
    string line;
    while (getline(cin, line))
        moves += line;

    return moves;
}

void printGrid(const vector<vector<char>>& grid, int r, int c) {
    for (int i=0; i<grid.size(); ++i) {
        for (int j=0; j<grid[0].size(); ++j) {
            if (i == r && j == c) cout << '@';
            else cout << grid[i][j];
        }
        cout << endl;
    }
    cout << endl;
}

void shiftUp(int r, int c, vector<vector<char>>& grid) {
    int pushR = r-1;
    while (pushR >= 0) {
        if (grid[pushR][c] == '#') return; // no push space found
        if (grid[pushR][c] == '.')
            break;
        pushR --;
    }
    char prev = grid[r-1][c];
    grid[r-1][c] = '.';
    for (int k=r-2; k >= pushR; k--) {
        char temp = grid[k][c];
        grid[k][c] = prev;
        prev = temp;
    }
}

void shiftDown(int r, int c, vector<vector<char>>& grid) {
    int pushR = r+1;
    while (pushR < grid.size()) {
        if (grid[pushR][c] == '#') return; // no push space found
        if (grid[pushR][c] == '.')
            break;
        pushR++;
    }
    char prev = grid[r+1][c];
    grid[r+1][c] = '.';
    for (int k=r+2; k <= pushR; k++) {
        char temp = grid[k][c];
        grid[k][c] = prev;
        prev = temp;
    }
}

void shiftRight(int r, int c, vector<vector<char>>& grid) {
    int pushC = c+1;
    while (pushC < grid[0].size()) {
        if (grid[r][pushC] == '#') return; // no push space found
        if (grid[r][pushC] == '.')
            break;
        pushC++;
    }
    char prev = grid[r][c+1];
    grid[r][c+1] = '.';
    for (int k=c+2; k <= pushC; k++) {
        char temp = grid[r][k];
        grid[r][k] = prev;
        prev = temp;
    }
}

void shiftLeft(int r, int c, vector<vector<char>>& grid) {
    int pushC = c-1;
    while (pushC >= 0) {
        if (grid[r][pushC] == '#') return; // no push space found
        if (grid[r][pushC] == '.')
            break;
        pushC--;
    }
    char prev = grid[r][c-1];
    grid[r][c-1] = '.';
    for (int k=c-2; k >= pushC; k--) {
        char temp = grid[r][k];
        grid[r][k] = prev;
        prev = temp;
    }
}

unordered_map<char, pair<int,int>> movement = {{'v', {1,0}}, {'^', {-1,0}}, {'<', {0,-1}}, {'>', {0,1}}};

int part1(vector<vector<char>> grid, const string& moves, int r, int c) {
    for (char m : moves) {
        auto nextMove = movement[m];
        int nextR = r + nextMove.first;
        int nextC = c + nextMove.second;

        if (grid[nextR][nextC] == 'O') { // try moving rock
            switch(m) {
                case '^':
                    shiftUp(r, c, grid); break;
                case 'v':
                    shiftDown(r, c, grid); break;
                case '>':
                    shiftRight(r, c, grid); break;
                case '<':
                    shiftLeft(r, c, grid); break;
            }
        }
        // if next spot opened up, move there
        if (grid[nextR][nextC] == '.') {
            r = nextR;
            c = nextC;
        }
        // cout << "Move " << m << ":" << endl;
        // printGrid(grid, r, c);
    }

    // count scores
    int score = 0;
    for (int i=0; i<grid.size(); ++i) {
        for (int j=0; j<grid.size(); ++j) {
            if (grid[i][j] == 'O')
                score += 100 * i + j;
        }
    }
    return score;
}

vector<vector<char>> expandGrid(const vector<vector<char>>& grid, int& startR, int& startC) {
    vector<vector<char>> newGrid;
    startC *= 2;

    for (const auto& row : grid) {
        vector<char> newRow;
        for (const auto& c : row) {
            if (c == '#') { newRow.emplace_back('#'); newRow.emplace_back('#'); }
            else if (c == 'O') { newRow.emplace_back('['); newRow.emplace_back(']'); }
            else if (c == '.') { newRow.emplace_back('.'); newRow.emplace_back('.'); }
        }
        newGrid.emplace_back(newRow);
    }
    return newGrid;
}

bool isMoveable(vector<vector<char>>&  grid, int r, int c, const pair<int,int>& move, set<pair<int,int>>& visited) {
    int nr = r + move.first;
    int nc = c + move.second;

    if (visited.find({r,c}) != visited.end())
        return true;

    visited.insert({r,c});

    switch(grid[nr][nc]) {
        case '#':
            return false;
        case '[':
            return isMoveable(grid, nr, nc, move, visited) && isMoveable(grid, nr, nc+1, move, visited);
        case ']':
            return isMoveable(grid, nr, nc, move, visited) && isMoveable(grid, nr, nc-1, move, visited);
    }
    return true;

}

bool doMove(vector<vector<char>>&  grid, int r, int c, char move) {
    auto m = movement[move];
    int nr = r + m.first;
    int nc = c + m.second;

    vector<vector<char>> oldGrid = grid;

    if (grid[nr][nc] == '[' || grid[nr][nc] == ']') {
        set<pair<int,int>> visited;

        // check if it is moveable, by CHECKING BOTH PAIRS OF []
        if (!isMoveable(grid, r, c, movement[move], visited)) return false;

        // move all visited blocks by the given amount
        for (auto& p : visited) { // erase
            grid[p.first][p.second] = '.';
        }
        for (auto& p : visited) { // redraw
            grid[p.first + movement[move].first][p.second + movement[move].second] = oldGrid[p.first][p.second];
        }

    } else if (grid[nr][nc] == '#') return false;
    
    swap(grid[r][c], grid[nr][nc]);
    return true;
}

int part2(const vector<vector<char>>&  gridSmall, int r, int c, const string& moves) {
    vector<vector<char>> grid = expandGrid(gridSmall, r, c);

    for (char m : moves) {
        bool succ = doMove(grid, r, c, m); // recursively checks if this move is possible

        if (succ) {
            r += movement[m].first;
            c += movement[m].second;
        }
        // cout << "Move " << m << ":" << endl;
        // printGrid(grid, r, c);
    }

    int score = 0;
    for (int i=0; i<grid.size(); ++i) {
        for (int j=0; j<grid[0].size(); ++j) {
            if (grid[i][j] == '[') score += 100* i + j;
        }
    }
    return score;
}

int main() {
    int startR, startC;
    vector<vector<char>> grid = parseGrid(startR, startC);
    string moves = parseMoves();
    
    int p1 = part1(grid, moves, startR, startC);
    cout << "part1: " << p1 << endl;

    int p2 = part2(grid, startR, startC, moves);
    cout << "part2: " << p2 << endl;
}
