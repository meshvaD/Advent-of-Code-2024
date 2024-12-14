#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <set>
using namespace std;

struct Bot {
    int x, y, vx, vy;
};

int ROWS = 103;
int COLS = 101;

vector<Bot> parse() {
    vector<Bot> bots;
    string line;
    fstream inFile("input/14.txt");

    while (getline(inFile, line)) {
        stringstream ss(line);
        char c;

        while (ss >> c && c != '=');
        int x; ss >> x;
        ss >> c; // skip ','
        int y; ss >> y;

        while (ss >> c && c != '=');    
        int vx; ss >> vx;
        ss >> c;// skip ','
        int vy; ss >> vy;

        bots.emplace_back(Bot{x, y, vx, vy});
    }
    return bots;
}

pair<int,int> evalFinalPos(const Bot& bot, int steps, int rows=ROWS, int cols=COLS) {
    int new_x = (bot.x + bot.vx * steps) % cols;
    if (new_x < 0) new_x += cols;

    int new_y = (bot.y + bot.vy * steps) % rows;
    if (new_y < 0) new_y += rows;

    return {new_x, new_y};
}

void tick(Bot& bot, int rows=ROWS, int cols=COLS) {
    bot.x = (bot.x + bot.vx) % cols;
    if (bot.x < 0) bot.x += cols;

    bot.y = (bot.y + bot.vy) % rows;
    if (bot.y < 0) bot.y += rows;
}

void viz(const vector<vector<int>>& grid, ostream& out) {
    for (const auto& row : grid) {
        for (const auto& c : row) {
            if (c == 0) out << '.';
            else out << c;
        }
        out << endl;
    }
    out << endl;
}

int getQuad(const pair<int,int>& coord, int rows=ROWS, int cols=COLS) {
    // coords in format (x, y) = (c, r)
    if (coord.first < cols/2) {
        if (coord.second < rows/2) return 0;
        else if (coord.second > rows/2) return 1;
    } else if (coord.first > cols/2) {
        if (coord.second < rows/2) return 2;
        else if (coord.second > rows/2) return 3;
    }

    return -1; // part of no quad
}

int part1(const vector<Bot>& bots) {
    int steps = 100;

    // vector<vector<int>> grid(7, vector<int>(11, 0));
    vector<int> quadCounts(4, 0);

    for (const Bot& b : bots) {
        pair<int,int> coords = evalFinalPos(b, steps); // return (x, y) which is (c, t)
        int quad = getQuad(coords);
        if (quad >= 0) quadCounts[quad] += 1;

        // grid[coords.second][coords.first] += 1;
    }

    int prod = 1;
    for (const auto c : quadCounts) prod *= c;
    return prod;
}

void part2(vector<Bot> bots) {
    // no overlapping bots??

    for (int i=1; i<10000; ++i) {
        cout << "iter: " << i << endl;
        ofstream outFile("dump.txt", std::ios::trunc);
        vector<vector<int>> grid(ROWS, vector<int>(COLS, 0));

        // set<pair<int,int>> placed;
        bool found = true;

        for (Bot& b : bots) {
            tick(b);
            grid[b.y][b.x] += 1;

            if (grid[b.y][b.x] > 1) {
                found = false;
            }
        }
        if (found) {
            cout << "FOUND" << endl;
            viz(grid, outFile);
            break;
        }
        // char c; 
        // cin.get(c); // wait for someone to check there is no tree & continue
    }

}

int main() {
    vector<Bot> bots = parse();
    int p1 = part1(bots);

    cout << "part1: " << p1 << endl;

    part2(bots);
}
