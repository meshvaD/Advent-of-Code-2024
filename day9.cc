#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
using namespace std;

uint64_t consecSum(uint64_t from, uint64_t to) {
    uint64_t a = to * (to+1) / 2;
    uint64_t b = (from - 1) * (from) / 2;
    return a - b;
}

// 12345
uint64_t getChecksumP1(vector<uint64_t> input) {
    uint64_t checksum = 0;
    uint64_t fIndex = 0;

    for (int i=0; i<input.size(); ++i) {
        if (i % 2 == 0) { // represents a file of block size
            checksum += (i/2) * consecSum(fIndex, input[i] + fIndex - 1);
            fIndex += input[i];
        } else {
            while (input[i] > 0 && input.size() > i+1) {
                // get last non-empty val
                uint64_t loc = input.size()-1;

                for (int j=0; j<input[i]; ++j) {
                    if (input[loc] <= 0) break;
                    input[loc] -= 1;
                    input[i] -= 1; // one less free space to fill

                    checksum += uint64_t(loc/2) * (fIndex);
                    fIndex ++;
                }

                if (input[loc] == 0) {
                    input.pop_back(); input.pop_back();
                }
            }
        }
    }
    return checksum;
}

// 00 99[1] 2111777.44.333....5555.6666..... 8888[16] ..
// TODO PART 2 : https://github.com/jwezorek/advent_of_code/blob/main/src/2024/day_09.cpp

class FreeList {
    struct Info {
        uint64_t start, cap;
    };

    list<Info> freeList;

    uint64_t addToList(uint64_t id, uint64_t cap, uint64_t prefixSum, uint64_t& checksum) {
        int start = prefixSum;
        for (auto it = freeList.begin(); it != freeList.end(); ++it) {
            if (it->cap >= cap) {
                if (it->start >= prefixSum) break;
                // cout << "found match: " << it->cap << " idx: " << it->start << " req: " << cap << endl;

                // reduce cap of occupied block
                start = it->start;
                it->start += cap;
                it->cap -= cap;
                if (it->cap == 0) freeList.erase(it);

                // add new empty block @ start pos of the moved block
                Info newInfo{prefixSum, cap};
                auto newInsertPos = freeList.end();
                for (auto addIt = freeList.begin(); addIt != freeList.end(); ++addIt) {
                    if (addIt->start > prefixSum) {
                        newInsertPos = addIt;
                        break;
                    }
                }
                freeList.insert(newInsertPos, newInfo);
                std::advance(newInsertPos, -1);

                // COLLAPSE
                // if (id == 4) break;

                auto lookAroundIt = newInsertPos;
                std::advance(lookAroundIt, -1); // previous
                // cout << "prev: " << lookAroundIt->start + lookAroundIt->cap << " == " << newInsertPos->start << endl;
                
                if (newInsertPos != freeList.begin() && lookAroundIt->start + lookAroundIt->cap == newInsertPos->start) {
                    newInsertPos->start = lookAroundIt->start;
                    newInsertPos->cap += lookAroundIt->cap;

                    freeList.erase(lookAroundIt);
                    // cout << "FRONT" << endl;
                }
                lookAroundIt = newInsertPos;
                std::advance(lookAroundIt, 1); // next
                // cout << "next: " << lookAroundIt->start << " curr: " << newInsertPos->start << endl;
                if (lookAroundIt != freeList.end() && newInsertPos->start + newInsertPos->cap == lookAroundIt->start) {
                    newInsertPos->cap += lookAroundIt->cap;

                    freeList.erase(lookAroundIt);

                    // cout << "BACK" << endl;
                }

                break;
            }
        }

        // update checksum
        return id * consecSum(start, start + cap - 1);
    }

public:

    uint64_t packFiles(const vector<uint64_t>& inputs) {
        // add to free list
        uint64_t sum = 0;
        vector<uint64_t> prefixSums;
        for (int i=0; i<inputs.size(); ++i) {            
            prefixSums.emplace_back(sum);
            if (i % 2 == 1 && inputs[i] > 0) freeList.push_back(Info{sum, inputs[i]});
            sum += inputs[i];
        }

        // iter over all filled spaces
        uint64_t checksum = 0;

        for (int i=inputs.size()-1; i >= 0; --i) {
            if (i % 2 == 0) {
                checksum += addToList(i/2, inputs[i], prefixSums[i], checksum);

                // print free list
                // cout << "adding: " << i/2 << " : " << inputs[i] << endl;
                // for (const auto& v : freeList)
                //     cout << v.start << " : " << v.cap << "  ->  ";
                // cout << endl;

                // if (i/2 == 4) break;
            }
        }

       return checksum;
    }
};

int main() {
    char ch;
    vector<uint64_t> inputs;
    while (cin >> ch) inputs.emplace_back(ch - '0');

    cout << "part1: " << getChecksumP1(inputs) << endl;

    FreeList f;
    cout << "part2: " << f.packFiles(inputs) << endl;
}
