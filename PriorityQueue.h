//
// Created by sagy on 1/14/19.
//

#ifndef SERVER_PRIORITYQUEUE_H
#define SERVER_PRIORITYQUEUE_H

#include <Searchable/State.h>
#include <queue>

template <class Node>
class PriorityQueue {
    std::priority_queue<State<Node> *,
            std::vector<State<Node> *>,
                    Compare<Node>> queue;

public:
    PriorityQueue(){
        this->queue = std::priority_queue<State<Node> *,
                std::vector<State<Node> *>,
                        Compare<Node>>();
    }

    bool empty() { return this->queue.empty(); }

    unsigned long size() { return this->queue.size(); }

    void addToPQ(State<Node> *s) { this->queue.push(s); }

    State<Node>* popFromPQ() {
        State<Node> *temp = this->queue.top();
        this->queue.pop();
        return temp;
    }

    bool contains(State<Node> *s) {
        std::priority_queue<State<Node> *,
                std::vector<State<Node> *>,
                        Compare<Node>> tempQueue = this->queue;
        while (!tempQueue.empty()) {
            State<Node> *tempState = tempQueue.top();
            tempQueue.pop();
            if (tempState == s) {
                return true;
            }
        }
        return false;
    }

    void updatePriority(State<Node> *s, double betterPath,
            State<Node> *father) {
        std::vector<State<Node> *> temp;
        while (this->queue.top() != s) {
            temp.push_back(this->queue.top());
            this->queue.pop();
        }
        State<Node> *updated = this->queue.top();
        this->queue.pop();
        // update to the better pathCost, and update Camefrom
        updated->setPathCost(betterPath);
        updated->setParent(father);
        for (int i = 0; i < temp.size(); ++i) {
            // push back all the states that were popped
            this->queue.push(temp.at(i));
        }
        // push the updated state
        this->queue.push(updated);
    }
};


#endif //SERVER_PRIORITYQUEUE_H
