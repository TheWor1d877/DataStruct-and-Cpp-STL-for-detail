#include "SkipList.h"
#include <set>
#include <vector>
#include <random>
#include <chrono>
#include <climits>
#include <iostream>
#include <fstream>
#include <functional>

using namespace std::chrono;

// 计时器：返回毫秒
double timeit(const std::function<void()>& fn) {
    auto start = high_resolution_clock::now();
    fn();
    auto end = high_resolution_clock::now();
    return duration<double, std::milli>(end - start).count();
}

// 生成随机数据
std::vector<int> generateRandom(size_t n) {
    std::mt19937 rng(42);  // 固定种子，结果可复现
    std::uniform_int_distribution<int> dist(0, INT_MAX);
    std::vector<int> data(n);
    for (auto& x : data) x = dist(rng);
    return data;
}

// 生成有序数据
std::vector<int> generateSorted(size_t n) {
    auto data = generateRandom(n);
    std::sort(data.begin(), data.end());
    return data;
}

// 测试用例模板
struct TestResult {
    std::string name;
    size_t n;
    double skipInsert, skipFind, skipErase;
    double setInsert, setFind, setErase;
};

void runBenchmark(TestResult& r) {
    auto data = generateRandom(r.n);
    std::set<int> stdSet;
    SkipList skip;

    // 1. 插入
    r.skipInsert = timeit([&] {
        for (int x : data) skip.insert(x);
    });
    r.setInsert = timeit([&] {
        for (int x : data) stdSet.insert(x);
    });

    // 2. 查找（测 10% 的数据）
    bool res1;
    std::set<int>::iterator res2;
    auto queries = generateRandom(r.n / 10);
    r.skipFind = timeit([&] {
        for (int x : queries) res1 = skip.find(x);
    });
    r.setFind = timeit([&] {
        for (int x : queries) res2 = stdSet.find(x);
    });

    // 3. 删除（删 10% 的数据）
    r.skipErase = timeit([&] {
        for (int x : queries) skip.erase(x);
    });
    r.setErase = timeit([&] {
        for (int x : queries) stdSet.erase(x);
    });
}


void printMarkdown(const std::vector<TestResult>& results) {
    std::cout << "| 操作 | 数据量 | SkipList (ms) | std::set (ms) | 比值 |\n";
    std::cout << "|------|--------|---------------|---------------|------|\n";
    for (const auto& r : results) {
        std::cout << "| 插入 | " << r.n << " | " << r.skipInsert << " | " << r.setInsert
                  << " | " << r.skipInsert / r.setInsert << " |\n";
        std::cout << "| 查找 | " << r.n << " | " << r.skipFind << " | " << r.setFind
                  << " | " << r.skipFind / r.setFind << " |\n";
        std::cout << "| 删除 | " << r.n << " | " << r.skipErase << " | " << r.setErase
                  << " | " << r.skipErase / r.setErase << " |\n";
    }
}

// CSV 导出
void exportCSV(const std::vector<TestResult>& results, const std::string& path) {
    std::ofstream csv(path);
    csv << "操作,数据量,SkipList,set\n";
    for (const auto& r : results) {
        csv << "插入," << r.n << "," << r.skipInsert << "," << r.setInsert << "\n";
        csv << "查找," << r.n << "," << r.skipFind << "," << r.setFind << "\n";
        csv << "删除," << r.n << "," << r.skipErase << "," << r.setErase << "\n";
    }
}
