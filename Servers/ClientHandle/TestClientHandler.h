//
// Created by sagy on 1/6/19.
//

#ifndef SERVER_MYTESTCLIENTHANDLER_H
#define SERVER_MYTESTCLIENTHANDLER_H


#include <CommandReader/CommandReader.h>
#include <unistd.h>
#include <cstring>
#include "ClientHandler.h"
#include "Solver.h"
#include "CommandReader/CommandLineReader.h"

template <class Problem, class Solution>
class TestClientHandler : public ClientHandler {
    Solver<Problem,Solution>* solver;
    int client_num;
    //CacheManager cm ;
public:
    TestClientHandler(Solver<Problem,Solution>*
            sol){
        this->solver = sol;
    }


    virtual void handleClient(int port )override{
        CommandReader* commandReader = new CommandLineReader(port);
        std::string problem = commandReader->read();
        client_num++;
        std::cout << "Problem: " << problem << std::endl;
		std::cout << "Client number: " << client_num << std::endl;

		const char* solution = solver->solve(problem).c_str();
		int n = ::write(port, solution, strlen(solution));

		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}
    }
};


#endif //SERVER_MYTESTCLIENTHANDLER_H
