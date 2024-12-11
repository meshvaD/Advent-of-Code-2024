#include <vector>
#include <iostream>
#include <math.h>
#include <unordered_map>
using namespace std;

struct hash_pair {
    size_t operator()(const pair<uint64_t,uint64_t>& p) const
    {
        return (p.first << 16) | p.second;
    }
};

class Rocks {
    vector<uint64_t> rocks;
    unordered_map<pair<uint64_t, uint64_t>, uint64_t, hash_pair> memo;

    uint64_t dp(uint64_t val, uint64_t n) {
        // cout << val << " , " << n << endl;
        if (n == 0) return 1; // return the count (if reached level=0 no more processing, it is a single rock)

        if (auto it = memo.find({val, n}); it != memo.end()) {
            return it->second;
        }
        
        uint64_t ret = 0;
        if (val == 0) ret = dp(1, n-1);
        else if (uint64_t num_digs = uint64_t(log10(val)) + 1; val != 0 && num_digs % 2 == 0) {
            uint64_t p = pow(10, num_digs/2);
            ret = dp(val / p, n-1) + dp(val % p, n-1);
        } else {
            ret = dp(val * 2024, n-1);
        }

        memo[{val, n}] = ret;
        return ret;
    }

public:
    void print() {
        for (const auto& it : rocks) cout << it << " , ";
        cout << endl;
    }

    uint64_t part(int times) {
        uint64_t sum = 0;
        for (int i=0; i<rocks.size(); ++i) {
            sum += dp(rocks[i], times);
        }
        return sum;
    }

    Rocks() {
        int n;
        while (cin >> n) rocks.emplace_back(n);
    }
};

int main() {
    Rocks rocks;
    rocks.print();

    uint64_t p1 = rocks.part(25);
    cout << "part1: " << p1 << endl;

    uint64_t p2 = rocks.part(75);
    cout << "part2: " << p2 << endl;
}
