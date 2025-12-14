#ifndef SKIP_LIST_H_
#define SKIP_LIST_H_

#include <vector>
#include <random>

class SkipList {
    struct Node {
        int key_;
        std::vector<Node*> next_;
        explicit Node(int k, int level);
        Node();
    };

    static constexpr int MAX_LEVEL = 16;
    static constexpr double P = 0.5;

    Node* head_;
    int current_level_;

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
