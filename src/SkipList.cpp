#include "SkipList.h"
#include<vector>
#include<random>
#include<iostream>

int SkipList::randomLevel() {
    int level = 1;
    while (dist_(rng_) < P && level < MAX_LEVEL) {
        ++level;
    }
    return level;
}

SkipList::SkipList():
currentLevel_(1),
rng_(std::random_device{}()),
dist_(0,1){
    head_ = new Node();
}

SkipList::~SkipList(){
    const Node* cur = head_;
    while(cur != nullptr){
        const Node* next = cur->next_[0];
        delete cur;
        cur = next;
    }
}

bool SkipList::find(int key) const{
    const Node* cur = head_;
    for(int i=currentLevel_-1;i>=0;i--){
        while(cur->next_[i] && cur->next_[i]->key_ < key){
            cur = cur->next_[i];
        }
    }
    cur = cur->next_[0];
    return (cur && cur->key_ == key);
}

void SkipList::insert(int key){
    std::vector<Node*> update(MAX_LEVEL,nullptr);
    Node* cur = head_;
    for(int i=currentLevel_-1;i>=0;i--){
        while(cur->next_[i] && cur->next_[i]->key_ < key){
            cur = cur->next_[i];
        }
        update[i] = cur;
    }

    if(cur->next_[0] && cur->next_[0]->key_ == key) return;

    const int nodeLevel = randomLevel();

    if(nodeLevel > currentLevel_){
        for(int i=currentLevel_;i<nodeLevel;i++){
            update[i] = head_;
        }
        currentLevel_ = nodeLevel;
    }

    Node* newNode = new Node(key,nodeLevel);
    for(int i=0;i<nodeLevel;i++){
        newNode->next_[i] = update[i]->next_[i];
        update[i]->next_[i] = newNode;
    }
}

bool SkipList::erase(int key){
    std::vector<Node*> update(MAX_LEVEL,nullptr);
    Node* cur = head_;
    for(int i=currentLevel_-1;i>=0;i--){
        while(cur->next_[i] && cur->next_[i]->key_ < key){
            cur = cur->next_[i];
        }
        update[i] = cur;
    }

    Node* target = cur->next_[0];
    if(target == nullptr || target->key_ != key) return false;

    for(int i=0;i<currentLevel_;i++){
        if(update[i]->next_[i] != target) break;
        update[i]->next_[i] = target->next_[i];
    }
    delete target;

    while(currentLevel_ > 1 && head_->next_[currentLevel_-1] == nullptr){
        currentLevel_ --;
    }
    return true;
}