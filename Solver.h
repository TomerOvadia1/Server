//
// Created by sagy on 1/6/19.
//

#ifndef SERVER_SOLVER_H
#define SERVER_SOLVER_H

template <class Problem, class Solution>
class Solver {
public:
    virtual Solution solve(Problem prob) = 0;
    virtual ~Solver() {}
};


#endif //SERVER_SOLVER_H
