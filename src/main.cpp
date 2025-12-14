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

    // 1. 控制台输出（直接复制到 Markdown）
    printMarkdown(results);

    // 2. 导出 CSV
    exportCSV(results, "benchmark.csv");
    std::cout << "\n已导出 benchmark.csv，用 Excel 生成柱状图\n";

    return 0;
}