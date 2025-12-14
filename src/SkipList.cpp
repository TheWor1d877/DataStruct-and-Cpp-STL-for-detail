#include<vector>
#include<random>
#include<climits>
#include<iostream>

/**
跳表节点定义
每个节点包含：
key：存储的值
next_：多层 forward 指针，next_[i] 表示第 i 层的下一个节点
 */
class SkipList{
    struct Node{
        int key_;
        std::vector<Node*> next_;
        explicit Node(int k,int level) : key_(k),next_(level,nullptr) {};
        explicit Node() : Node(INT_MIN,MAX_LEVEL) {};
    };

    // 最大层数
    static constexpr int MAX_LEVEL = 16;
    // 节点晋升概率
    static constexpr double P = 0.5;
    // 头节点（哨兵节点，不存储真实数据）
    Node* head_;
    // 当前跳表实际使用的最高层数
    int currentLevel_;

    // 随机数引擎
    std::mt19937 rng_;
    std::uniform_real_distribution<double> dist_;

    /**
    随机生成节点层数
    通过“抛硬币”方式决定是否晋升到更高层
     */
    int randomLevel() {
        int level = 1;
        while (dist_(rng_) < P && level < MAX_LEVEL) {
            ++level;
        }
        return level;
    }

public:
    SkipList():
    currentLevel_(1),
    rng_(std::random_device{}()),
    dist_(0,1){
        head_ = new Node();
    }

    ~SkipList(){
        const Node* cur = head_;
        while(cur != nullptr){
            const Node* next = cur->next_[0];
            delete cur;
            cur = next;
        }
    }

    /**
    查找 key 是否存在
    时间复杂度：O(log n)（期望）
     */
    [[nodiscard]] bool find(const int value) const{ /* const: 不能通过find函数来修改成员变量*/
        const Node* cur = head_;
        // 从最高层开始向下查找
        for(int i=currentLevel_-1;i>=0;i--){
            while(cur->next_[i] && cur->next_[i]->key_ < value){ //find
                cur = cur->next_[i];
            }
        }
        cur = cur->next_[0];
        return (cur && cur->key_ == value);
    }

    /*
    插入一个 key
    如果 key 已存在，则忽略
     */
    void insert(const int key){
        // update[i] 记录第 i 层中，待插入位置的前驱节点
        std::vector<Node*> update(MAX_LEVEL,nullptr);
        Node* cur = head_;
        for(int i=currentLevel_-1;i>=0;i--){
            while(cur->next_[i] && cur->next_[i]->key_ < key){
                cur = cur->next_[i];
            }
            update[i] = cur;
        }

        // 如果 key 已存在，直接返回
        if(cur->next_[0] && cur->next_[0]->key_ == key) return;

        //随机生成节点应该有的层数
        const int nodeLevel = randomLevel();
        std::cout<<"new node layer:"<<nodeLevel<<std::endl;

        /*
        如果新节点层数高于当前层数，需要更新头节点
        (头节点的高度等于所有节点中最高的那个节点)
        如原本是3层索引，现在随机数生成一个4层，就需要动头节点
        */
        if(nodeLevel<currentLevel_){
            for(int i=currentLevel_;i>nodeLevel;i--){
                update[i] = head_;
            }
            currentLevel_ = nodeLevel;
        }

        //链表的插入
        const auto newNode = new Node(key,nodeLevel);
        /*
         * const auto newNode = ... → Node* const（指针不可重指向）
         * const auto* newNode = ... → const Node*（内容不可改)
         */
        for(int i=0;i<nodeLevel;i++){
            newNode->next_[i] = update[i]->next_[i];
            update[i]->next_[i] = newNode;
        }
    }

    /**
    删除一个 key
    成功返回1,
    不存在就是失败，返回0
     */
    bool erase(const int value){
        //update 存储前区节点
        std::vector<Node*> update(MAX_LEVEL,nullptr);
        Node* cur = head_;

        //与前面一样，现维护update这个vector
        for(int i=currentLevel_-1;i>=0;i--){
            while(cur->next_[i] && cur->next_[i]->key_ < value){
                cur = cur->next_[i];
            }
            update[i] = cur;
        }

        const Node* target = cur->next_[0];
        //没找到，返回false
        if(target == nullptr || target->key_ != value) return false;

        // 逐层断开节点，如果发现节点的next_不是target了，就说明target已经到达最大层了，就break
        for(int i=0;i<currentLevel_;i++){
            if(update[i]->next_[i] != target) break;
            update[i]->next_[i] = target->next_[i];
        }
        delete target;

        //更新头节点
        while(currentLevel_ > 1 && head_->next_[currentLevel_-1] == nullptr){
            currentLevel_ --;
        }
        return true;
    }
};


