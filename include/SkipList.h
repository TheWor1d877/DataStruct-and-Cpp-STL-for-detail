#ifndef SKIP_LIST_H_
#define SKIP_LIST_H_

#include <climits>
#include <vector>
#include <random>

class SkipList {
    struct Node {
        int key_;
        std::vector<Node*> next_;
        explicit Node(int k, int level) : key_(k), next_(level, nullptr) {}  // 内联实现
        explicit Node() : Node(INT_MIN, MAX_LEVEL) {}  // 内联实现
    };

    static constexpr int MAX_LEVEL = 16;
    static constexpr double P = 0.25;

    Node* head_;
    int currentLevel_;  // 改成和 .cpp 一致

    std::mt19937 rng_;
    std::uniform_real_distribution<double> dist_;

    int randomLevel();

public:
    SkipList();
    ~SkipList();

    [[nodiscard]] bool find(int key) const;
    void insert(int key);
    bool erase(int key);
};

#endif // SKIP_LIST_H_