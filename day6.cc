#include <vector>
#include <iostream>
#include <unordered_set>
#include <sstream>
using namespace std;

// initially moving up
// up -> right -> down -> left CW order
vector<vector<int>> moveDir = {{-1,0}, {0,1}, {1,0}, {0,-1}};

struct Info {
    int r, c, dir;
};
bool operator== (const Info& a, const Info& b)
{
    return (a.r == b.r) && (a.c == b.c) && (a.dir == b.dir);
}

class Grid {
    vector<vector<char>> grid;

    struct hash_Info {
        size_t operator()(const Info& p) const
        {
            return (p.r << 16) | (p.c << 2) | p.dir;
        }
    };

    bool move(int& r, int& c, int& dir, unordered_set<Info, hash_Info>* visited) {
        // cout << "visit: " << r << " , " << c << " dir: " << dir << endl;

        int prevR = r;
        int prevC = c;

        // check if next step is out of bounds
        r += moveDir[dir][0];
        c += moveDir[dir][1];

        // out of bounds - end reached
        if (r < 0 || c < 0 || r >= grid.size() || c >= grid[0].size()) return true;

        // next place has an obstacle (cannot visit, make turn)
        if (grid[r][c] == '#') {
            dir = (dir + 1) % 4;
            r = prevR; c = prevC;
        } else if (visited) { // visit the place
            visited->insert(Info{r, c, 0});
        }
        return false;
    }

    bool checkLoop(int r, int c, int dir) {
        unordered_set<Info, hash_Info> visited;
        bool foundLoop = false;
        bool reachedEnd = false;

        visited.insert(Info{r,c, dir});

        while (!reachedEnd && !foundLoop) {
            int prevDir = dir;
            reachedEnd = move(r, c, dir, nullptr);
            if (prevDir != dir) { // changed dirs at this loc, add to set

                // if alr visited this corner (in a LOOP)
                if (visited.find(Info{r,c, dir}) != visited.end()) foundLoop = true;

                visited.insert(Info{r,c, dir});
            }
        }

        return foundLoop;
    }

public:
    Grid() {
    }

    int part1(int r, int c) {
        unordered_set<Info, hash_Info> visited;

        int dir = 0; // start here
        visited.insert(Info{r,c,0}); // visit the start pair
        bool reachedEnd = false;

        while (!reachedEnd){
            reachedEnd = move(r, c, dir, &visited);
        }
        return visited.size();
    }

    int part2(int startR, int startC) {
        int count = 0;
        for (int r=0; r<grid.size(); ++r) {
            for (int c=0; c<grid[0].size(); ++c) {
                if (grid[r][c] == '#' || (r == startR && c == startC)) continue; // if alr has obstacle there

                grid[r][c] = '#';

                bool loop = checkLoop(startR, startC, 0);
                if (loop) ++count;
                grid[r][c] = '.'; // undo
            }
        }
        return count;
    }

    pair<int,int> parseInput() {
        string s;
        int startR, startC;

        while (getline(cin, s)) {
            stringstream ss(s);
            char c;
            vector<char> row;
            while (ss >> c) {
                if (c == '^') {
                    startR = grid.size(); startC = row.size();
                }
                row.emplace_back(c);
            }

            grid.emplace_back(row);
        }
        return {startR, startC};
    }
};

int main() {
    Grid g;
    auto startPair = g.parseInput();
    cout << "startR: " << startPair.first << " startC: " << startPair.second << endl;

    int p1 = g.part1(startPair.first, startPair.second);
    cout << "part1: " << p1 << endl;

    int p2 = g.part2(startPair.first, startPair.second);
    cout << "part2: " << p2 << endl;
}
