#include <vector>
#include <iostream>
#include <sstream>
using namespace std;

struct Work {
    vector<int> A;
    vector<int64_t> B;
};

vector<Work> parse() {
    vector<Work> allWork;

    string line;
    vector<int> A(4);
    vector<int64_t> B(2);

    int filledA = 0;

    while (getline(cin, line)) {
        if (line.size() == 0) continue;
        stringstream ss(line);

        char c;
        if (filledA != 2) {
            while (ss >> c && c != '+') {}
            ss >> A[0 + filledA];

            while (ss >> c && c != '+') {}
            ss >> A[2 + filledA];

            filledA++;
        } else {
            while (ss >> c && c != '=') {}
            ss >> B[0];

            while (ss >> c && c != '=') {}
            ss >> B[1];

            filledA = 0;
            allWork.emplace_back(Work{A, B});
        }
    }

    // print all work
    /* for (const Work& w : allWork) {
        for (int i=0; i<4; ++i) cout << w.A[i] << " , ";
        cout << endl;
        for (int i=0; i<2; ++i) cout << w.B[i] << " , ";
        cout << endl;
    }
    */
   return allWork;
}

// expect 2x2 mat
int determinant(const vector<int>& mat) {
    return mat[0] * mat[3] - mat[1] * mat[2];
}

// solve AX = B
vector<int64_t> solve(const vector<int>& A, const vector<int64_t>& B) {
    int det = determinant(A);
    if (det == 0) return {0, 0}; // not possible

    int mult = det < 0 ? -1 : 1;
    det = abs(det);

    // X = 1/det * (A_inv * B), where [a b] = (A_inv * B)
    int64_t a = mult * (A[3] * B[0] - A[1] * B[1]);
    int64_t b = mult * (- A[2] * B[0] + A[0] * B[1]);

    if (a % det != 0 || b % det != 0) return {0,0};

    return {int64_t(a/det), int64_t(b/det)};
}

int part1(const vector<Work>& allWork) {
    int cost = 0;
    for (const Work& w : allWork) {
        vector<int64_t> ab = solve(w.A, w.B);
        cost += ab[0] * 3 + ab[1] * 1;
    }
    return cost;
}

int64_t part2(vector<Work>& allWork) {
    int64_t cost = 0;
    for (Work& w : allWork) {
        w.B[0] += 10000000000000;
        w.B[1] += 10000000000000;
        vector<int64_t> ab = solve(w.A, w.B);
        cost += ab[0] * 3 + ab[1] * 1;
    }
    return cost;
}

int main() {
    vector<Work> allWork = parse();  
    int p1 = part1(allWork); 
    cout << "part1: " << p1 << endl;

    int64_t p2 = part2(allWork); 
    cout << "part2: " << p2 << endl;
}
