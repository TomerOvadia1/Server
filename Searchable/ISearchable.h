//
// Created by sagy on 1/8/19.
//

#ifndef SERVER_ISEARCHABLE_H
#define SERVER_ISEARCHABLE_H

#include "State.h"
#include <vector>

template <class Node>
class ISearchable {
public:
    virtual State<Node>* getInitialState() = 0 ;
//    virtual State<Node> getGoalState()const = 0;
    virtual Node getInitialNode() const = 0;
    virtual Node getGoalNode() const = 0;
    virtual std::vector <State<Node>*> getAllPossibleStates(State<Node>* state)= 0;
    virtual std::vector<Node> getAllPossibleNodes(Node node)const = 0 ;

};


#endif //SERVER_ISEARCHABLE_H
