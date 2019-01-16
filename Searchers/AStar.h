//
// Created by sagy on 1/12/19.
//

#ifndef SERVER_ASTAR_H
#define SERVER_ASTAR_H

#include <list>
#include <algorithm>
#include "ISearcher.h"
#include "Searchable/ISearchable.h"
#include <unordered_map>
#include "Searchable/Matrix.h"
#include <string>
#include <algorithm>
#include <queue>


#include <list>
#include <algorithm>
#include "ISearcher.h"
#include "Searchable/ISearchable.h"
#include <unordered_map>
#include "Searchable/Matrix.h"
#include <string>
#include <algorithm>
#include <queue>

using namespace std;

template<class Node>
class AStar : public ISearcher<Node,vector<State<Node> *> > {

    class StateCompare {
        Node goal;
    public:
        StateCompare(Node g){
            this->goal=g;
        }
        using Point = std::pair<int,int>;
        double distance(Point cur, Point goal){
            int xDiff = abs(cur.first - goal.first);
            int yDiff = abs(cur.second - goal.second);
            return xDiff+yDiff;
        }

        bool operator()(State<Node> *left, State<Node> *right) {
            return left->getCost() + distance(left->getState(), this->goal) < right->getCost() +
            distance(right->getState(), this->goal);
        }
    };


    int nodesEvaluated = 0;
    vector<State<Node> *> saveValues;
    vector<State<Node> *> closed;
    vector<State<Node> *> temp;
    vector<State<Node> *> openList;


public:

    /**
     * Ctor
     */
    AStar() {
        this->nodesEvaluated = 0;
    }

    virtual vector<State<Node> *> search(ISearchable<Node> *searchable);

    virtual int getNumberOfNodesEvaluated() {
        return this->nodesEvaluated;
    }

    void addToOpenList(State<Node> *state){}

    int OpenListSize(){}

    virtual State<Node> *popOpenList(){}

    bool openContaines(State<Node> *){}

    State<Node> *bringContaines(State<Node> *state){}
};

template<class Node>
/**
 * The algorithm
 * @tparam Node
 * @param searchable
 * @return
 */
vector<State<Node> *> AStar<Node>::search(ISearchable<Node> *searchable) {
    closed.clear();

    openList.clear();

    openList.push_back(searchable->getInitialState());
    this->nodesEvaluated++;

    while (openList.size() > 0) {
        auto top_itr = min_element(openList.begin(), openList.end(), StateCompare(searchable->getGoalNode()));
        auto top = *top_itr;
        openList.erase(top_itr);

        closed.push_back(top);

        if (top->getState() == searchable->getGoalNode()) {
            return {top};
        }

        auto * mat = dynamic_cast<SearchableMatrix *>(searchable);

        for (State<Node> *&neighbor : mat->getAllPossibleStates(top)) {
            neighbor->setParent(top);
            neighbor->setCost(neighbor->getCost() + top->getCost());


            if (find_if(closed.begin(), closed.end(),
                        [neighbor](decltype(*begin(closed)) ptr )  {
                            return ptr->getState() == neighbor->getState();
                        }) != closed.end()) {
                continue;
            }

            auto itr = find_if(openList.begin(), openList.end(),
                               [neighbor](decltype(*begin(openList)) ptr)    {
                                   return ptr->getState() == neighbor->getState();
                               });

            if (itr != openList.end())  {
                (*itr)->setCost(min((*itr)->getCost(), top->getCost() + (*itr)->getCost()));
            } else{
                openList.push_back(neighbor);
                this->nodesEvaluated++;
            }
        }
    }
    return {};
}

#endif //SERVER_ASTAR_H
