//
// Created by sagy on 1/8/19.
//

#ifndef SERVER_ISEARCHER_H
#define SERVER_ISEARCHER_H

#include "Searchable/State.h"
#include "Searchable/ISearchable.h"
#include <vector>

template <class Node, class Solution>
class ISearcher {
public:
    virtual Solution search(ISearchable<Node> *searchable) = 0;
    virtual int getNumberOfNodesEvaluated() = 0;
    virtual ~ISearcher() {}
};


#endif //SERVER_ISEARCHER_H
