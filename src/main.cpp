#include "SkipList_test.h"
#include <vector>
#include <chrono>
#include <iostream>


int main() {
    std::vector<TestResult> results;
    std::vector<size_t> sizes = {10000, 100000, 1000000};

    for (size_t n : sizes) {
        TestResult r;
        r.n = n;
        r.name = "随机数据";
        runBenchmark(r);
        results.push_back(r);
    }

    printMarkdown(results);

    return 0;
}