#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
using namespace std;

template<typename T>
void quicksort(vector<T>& vec, int start, int end) {
    if (start+1 >= end) return;

    // choose partition value
    T part = vec[end-1];

    // partition (use swapping instead)
    int left = start;
    for (int i=start; i<end; ++i) {
        if (vec[i] < part) {
            // swap values
            T t = vec[i];
            vec[i] = vec[left];
            vec[left] = t;

            left += 1;
        }
    }

    // move pivot after smaller element
    vec[end-1] = vec[left];
    vec[left] = part;

    // recurse
    quicksort(vec, start, left);
    quicksort(vec, left+1, end);
}

int part1(vector<int> v1, vector<int> v2) {
    quicksort(v1, 0, v1.size());
    quicksort(v2, 0, v2.size());

    int sum = 0;
    for (int i=0; i<v1.size(); ++i) {
        sum += abs(v1[i] - v2[i]);
    }

    return sum;
}

unordered_map<int, int> getFreqMap(const vector<int>& vec) {
    unordered_map<int, int> freqMap;
    for (const int& val : vec) {
        if (auto iter = freqMap.find(val); iter != freqMap.end()) {
            iter->second++;
        } else {
            freqMap[val] = 1;
        }
    }
    return freqMap;
}

int part2(const vector<int>& v1, const vector<int>& v2) {
    int score = 0;
    unordered_map<int, int> freqMap = getFreqMap(v2);
    for (const auto& val : v1) {
        if (auto iter = freqMap.find(val); iter != freqMap.end())
            score += val * iter->second;
    }
    return score;
}

int main() {
    std::ifstream infile("input/1.txt");
    vector<int> v1,v2;

    int a, b;
    while (infile >> a >> b) {
        v1.emplace_back(a);
        v2.emplace_back(b);
    }

    cout << "part1: " << part1(v1, v2) << endl;
    cout << "part2: " << part2(v1, v2) << endl;
}
