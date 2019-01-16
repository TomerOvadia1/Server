//
// Created by tomer on 1/13/19.
//
#ifndef SERVER_SEARCINGSOLVERS_H
#define SERVER_SEARCINGSOLVERS_H

#include <Searchers/ISearcher.h>
#include "Solver.h"

template <class Node,class Solution>
class SearchingSolvers : public Solver<ISearchable<Node>*, Solution>{
	ISearcher<Node,Solution>* searcher;
	int numberOfNodesEvaluated;

public:
	SearchingSolvers(ISearcher<Node,Solution>* sr){
		this->searcher = sr;
	}

	virtual Solution solve(ISearchable<Node>* prob)  {
		Solution solution = searcher->search(prob);
		numberOfNodesEvaluated = searcher->getNumberOfNodesEvaluated();
		return solution;
	}

	int getNumberOfNodesEvaluated(){
		return numberOfNodesEvaluated;
	}

};
#endif //SERVER_SEARCINGSOLVERS_H
