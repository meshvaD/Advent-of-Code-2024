#include <string>
#include <iostream>
#include <regex>
#include <fstream>
#include <sstream>
using namespace std;

// mul\([0-9]*,[0-9]*\)

int parseSingleMult(string match) {
    stringstream ss(match);
    char c;
    int mult1, mult2;
    for (int i=0; i<4; ++i) ss >> c;
    ss >> mult1;
    ss >> c;
    ss >> mult2;

    // cout << mult1 << " " << mult2 << endl;
    return mult1 * mult2;
}

int part1(string s1) {
    regex e ("mul\\([0-9]*,[0-9]*\\)");

    std::sregex_iterator iter(s1.begin(), s1.end(), e);
    std::sregex_iterator end;

    int sum = 0;
    while(iter != end)
    {
        string match = (*iter)[0];
        sum += parseSingleMult(match);
        ++iter;
    }

    return sum;
}

int part2(string s1) {
    regex e ("mul\\([0-9]*,[0-9]*\\)|do\\(\\)|don't\\(\\)");

    std::sregex_iterator iter(s1.begin(), s1.end(), e);
    std::sregex_iterator end;

    int sum = 0;
    bool enable = true;
    while(iter != end)
    {
        string match = (*iter)[0];
        if (match == "do()") enable = true;
        else if (match == "don't()") enable = false;
        else if (enable) sum += parseSingleMult(match);
        ++iter;
    }

    return sum;
}

int main() {
    fstream file ("input/3.txt");
    string in, full;
    
    while (getline(file, in)) full += in;

    cout << "part1: " << part1(full) << endl;
    cout << "part2: " << part2(full) << endl;
}
