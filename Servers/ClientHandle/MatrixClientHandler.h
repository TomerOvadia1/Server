//
// Created by tomer on 1/12/19.
//

#ifndef SERVER_CLIENTHANDLERIML_H
#define SERVER_CLIENTHANDLERIML_H

#include <Solver.h>
#include <string>
#include <vector>
#include <Searchable/Matrix.h>
#include <CommandReader/CommandReader.h>
#include <CommandReader/CommandLineReader.h>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <algorithm>
#include "ClientHandler.h"
#include "LexerFunction.h"
#include "CacheManager/FileCacheManager.h"


const int POINT_SIZE = 2;
const int X_POS = 0 ;
const int Y_POS = 1 ;

class MatrixClientHandler : public ClientHandler {

	using Point = std::pair<int, int>;
	using MatrixSolver = Solver<ISearchable<Point>* ,std::vector<State<Point> *> > ;
	using MatrixVec = std::vector<std::vector<double>>;

	MatrixSolver* solver;

	FileCacheManager* cacheManager ;

	MatrixVec getMatrixVal(int port){
		std::vector<std::string> row_elements;
		std::string row;
		MatrixVec matrix_val;
		CommandReader *commandReader = new CommandLineReader(port);

		//get the first row_elements from user
		row = commandReader->read();

		//loop while user did not enter a terminator
		while (row.find("end") == std::string::npos) {
			row_elements = LexerFunction::lexer(row);
			matrix_val.push_back(StringVecConverter(row_elements));
			row = commandReader->read();
		}
		// row contains anything else
		int pos = row.find("end");
		std::string substring;
		for(int i=0 ; i<pos ;i++){
			substring+=row[i];
		}
		if(!substring.empty()){
			row_elements = LexerFunction::lexer(row);
			matrix_val.push_back(StringVecConverter(row_elements));
		}
		//delete command reader
		delete(commandReader);
		return matrix_val;
	}

	std::vector<double> StringVecConverter(std::vector<std::string>& stringVec){
		std::vector<double> row ;
		for(std::string& str : stringVec){
			double val = stoi(str);
			row.push_back(val);
		}
		return row;
	}

	Point extractPoint(MatrixVec& data){
		auto it = data.end() - 1;

		if(it->size() != POINT_SIZE){
			throw "Illegal Point";
		}
		//must get a matrix and 2 points
		if(it == data.begin()){
			throw "User did not enter an entry/exit points , or a matrix";
		}
		Point p = Point(it->at(X_POS),it->at(Y_POS));
		data.erase(it);
		return p;
	}

	std::string getPath(std::vector<Point> &solution){
		std::string path;
		Point parent;
		if(solution.empty()){
			return path;
		}
		parent = solution[0] ;
		int size = solution.size();
		for(int i=1 ; i< size; i++){
			Point cur_point = solution[i];
			//x position is smaller
			if(parent.first > cur_point.first){
				path+="Up";
			}else if(parent.first <cur_point.first){ //x position is larger
				path+="Down";
			}else if(parent.second > cur_point.second){	//y position is smaller
				path+="Left";
			}else if(parent.second < cur_point.second){//y position is larger
				path+="Right";
			}
			if(i != size -1){
				path+= ",";
			}
			parent = cur_point;
		}
		return path;
	}

	std::string getPath(std::vector<State<Point> *>& solution,State<Point> * initial){
		std::vector<Point> point_vec;
		if(solution.empty()){
			return "-1";
		}
		const State<Point> * cur_state = solution[0];
		while(*cur_state!= *initial){
			point_vec.push_back(cur_state->getState());
			cur_state = cur_state->getParent();
		}
		point_vec.push_back(cur_state->getState());
		std::reverse(point_vec.begin() , point_vec.end());
		return getPath(point_vec);
	}


public:
	MatrixClientHandler(MatrixSolver* sol,FileCacheManager* cm){
		this->solver = sol;
		this->cacheManager = cm ;
	}

	virtual void handleClient(int port )override {
		MatrixVec matrix_val;
		int PathSize;
		std::string path;

		matrix_val = getMatrixVal(port);

		//get initial and goal point
		Point goal = extractPoint(matrix_val);
		Point initial = extractPoint(matrix_val);

		//set up a searchable matrix ;
		Matrix basic_matrix = Matrix(matrix_val);
		auto matrix = new SearchableMatrix(basic_matrix, initial, goal);
		string prob = matrix->toString();

		if(cacheManager->is_solution_exists(prob)){
			std::cout << "Solution exist" << std::endl;
			path = cacheManager->getSolution(prob);
		}else{
			std::cout << "Solution DOES NOT exist" << std::endl;
			auto solution = solver->solve(matrix);
			path = getPath(solution, matrix->getInitialState());
			cacheManager->update_map(prob,path);
		}

		const char *to_print = path.c_str();
		int n = send(port, to_print, strlen(to_print), 0);
		if (n < 0) {
			perror("ERROR writing to socket");
			exit(1);
		}


		delete(matrix);

	}
};



#endif //SERVER_CLIENTHANDLERIML_H
