#include <vector>
#include <iostream>
#include <fstream>
#include <iostream>
using namespace std;

int searchGrid(const vector<vector<char>>& grid, string search, int row, int col, const vector<vector<int>> & movePos) {
    int count = 0;
    for (const auto& m : movePos) {
        int idx = 0;
        int new_row = row;
        int new_col = col;

        for (; idx < search.size(); ++idx) {
            if (new_row < 0 || new_col < 0 || new_row >= grid.size() || new_col >= grid[0].size()) break;

            if (grid[new_row][new_col] != search[idx]) break;
            new_row += m[0];
            new_col += m[1];
        }

        if (idx >= search.size()) count++;
    }
    return count;
}

int part1(const vector<vector<char>>& grid) {
    const vector<vector<int>> movePos = {{0,1},{0,-1},{1,0},{-1,0},{-1,-1},{1,1},{-1,1},{1,-1}};
    int count = 0;
    for (int r=0; r<grid.size(); ++r) {
        for (int c=0; c<grid[0].size(); ++c) {
            count += searchGrid(grid, "XMAS", r, c, movePos);
        }
    }
    return count;
}

int part2(const vector<vector<char>>& grid) {
    int count = 0;

    vector<vector<int>> diagAvails(grid.size(), vector<int>(grid[0].size(), -1));
    for (int r=1; r<grid.size()-1; ++r) {
        for (int c=1; c<grid[0].size()-1; ++c) {
            // (r,c) is the middle of the cross
            if (grid[r][c] != 'A') continue;

            int m_sum = (grid[r+1][c+1] == 'M') + (grid[r+1][c-1] == 'M') + (grid[r-1][c+1] == 'M') + (grid[r-1][c-1] == 'M');
            int s_sum = (grid[r+1][c+1] == 'S') + (grid[r+1][c-1] == 'S') + (grid[r-1][c+1] == 'S') + (grid[r-1][c-1] == 'S');
            if (m_sum == 2 && s_sum == 2 && grid[r+1][c+1] != grid[r-1][c-1]) ++count; 
        }
    }

    return count;
}

int main() {
    fstream file("input/4.txt");
    string line;
    vector<vector<char>> grid;
    while (getline(file, line)) {
        vector<char> row (line.begin(), line.end());
        grid.emplace_back(row);
    }

    cout << "part1: " << part1(grid) << endl;
    cout << "part2: " << part2(grid) << endl;
}
