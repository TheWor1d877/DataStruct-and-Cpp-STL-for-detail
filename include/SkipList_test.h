#ifndef DATA_STRUCT_TEST_H
#define DATA_STRUCT_TEST_H
#include <functional>
#include <string>
#include <vector>

double timeit(const std::function<void()>& fn);
std::vector<int> generateRandom(size_t n);
std::vector<int> generateSorted(size_t n);
struct TestResult {
    std::string name;
    size_t n;
    double skipInsert, skipFind, skipErase;
    double setInsert, setFind, setErase;
};
void runBenchmark(TestResult& r);
void printMarkdown(const std::vector<TestResult>& results);

#endif //DATA_STRUCT_TEST_H