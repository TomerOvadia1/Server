//
// Created by tomer on 1/10/19.
//

#ifndef SERVER_STRINGREVERSER_H
#define SERVER_STRINGREVERSER_H

#include "Solver.h"
#include <string>
#include <algorithm>

using namespace std;

class StringReverser : public Solver<string,string>{
public:
	string solve(string prob) override {
		std::reverse(prob.begin(), prob.end());
		return prob;

	}

};
#endif //SERVER_STRINGREVERSER_H
