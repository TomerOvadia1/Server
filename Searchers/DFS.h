//
// Created by sagy on 1/9/19.
//

#ifndef SERVER_DFS_H
#define SERVER_DFS_H

#include <map>
#include <queue>
#include "ISearcher.h"
#include <unordered_set>
#include <stack>

template <class Node>
class DFS : public ISearcher<Node, std::vector<Node>> {

    using Point = std::pair<int,int>;

    int nodesEvaluated;

    template <class Hashable>
    struct NodeHash {
    public:

        std::size_t operator()(const std::pair<int, int> &x) const
        {
            auto res = x.second + 0x9e3779b9 + (x.first << 6) + (x.second >> 2);
            res^=x.first;
            return std::hash<int>()(res);
        }


    };



public:
    DFS(){
        this->nodesEvaluated = 0;
    }


    virtual std::vector<Node> search(ISearchable<Node> *searchable) {
        // map to hold visited_map nodes
        std::stack<Node> open ;
        std::unordered_set<Node,NodeHash<Node>> visited ;
        std::unordered_set<Node,NodeHash<Node>> closed ;
        std::unordered_map<Node, std::vector<Node>,NodeHash<Node>> meta;
        Node goal = searchable->getGoalNode();


        // Mark the initial node as visited_map and enqueue it
        Node start = searchable->getInitialNode();

        //insert initial node to visited_map
        meta[start] = std::vector<Node>();
        this->nodesEvaluated++;


        //add initial node to queue
        open.push(start);
        visited.insert(start);

        while (!open.empty()) {
            // Dequeue a vertex from queue and print it
            Node father = open.top();

            if (father == goal) {
                vector<Node> route = meta[father];
                route.push_back(father);
                return route;
            }

            open.pop();

            std::vector<Node> adj = searchable->getAllPossibleNodes(father);

            // Get all adjacent vertices of the dequeued
            // vertex s. If a adjacent has not been visited_map,
            // then mark it visited_map and enqueue it
            for (int i = 0; i < adj.size(); ++i) {
                Node child = adj[i];
                if(closed.find(child) != closed.end()){
                    continue;
                }
                //if node not visited_map yet got visit
                if(visited.find(child) == visited.end()){
                    visited.insert(child);
                    open.push(child);
                    this->nodesEvaluated++;
                    vector<Node> route = meta[father];
                    route.push_back(father);
                    meta[child] = route;
                }

            }
            meta.erase(father);
        }
        return {};
    }

    virtual int getNumberOfNodesEvaluated(){
        return this->nodesEvaluated;
    }

//    std::vector<Node> backTrace(Node n, std::map<Node,Node> son_to_father){
//        std::vector<Node> path;
//        path.push_back(n);
//
//        while (son_to_father.find(n) != son_to_father.end()){
//            path.push_back(son_to_father.at(n));
//            n = son_to_father[n];
//        }
//        return path;
//    }
};



#endif //SERVER_DFS_H
