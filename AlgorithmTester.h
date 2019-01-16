//
// Created by tomer on 1/15/19.
//

#ifndef SERVER_ALGORITHMTESTER_H
#define SERVER_ALGORITHMTESTER_H
//
// Created by tomer on 1/13/19.
//
#include <string>
#include <cstring>
#include <vector>
#include "Servers/ClientHandle/LexerFunction.h"
#include <Searchable/Matrix.h>
#include <CommandReader/CommandReader.h>
#include <unistd.h>
#include <CommandReader/FileReader.h>
#include <iostream>
#include <Searchers/ISearcher.h>
#include <algorithm>
#include <CacheManager/FileCacheManager.h>

const int POINT_SIZE = 2;
const int X_POS = 0 ;
const int Y_POS = 1 ;
const char* TERMINATOR = "end";
const string FILENAME = "solution.txt";

template <class Solution>
class AlgorithmTester {
	using Point = std::pair<int,int>;
	using MatrixSearcher = ISearcher<Point,Solution> ;
	using MatrixVec = std::vector<std::vector<double>>;
	vector<pair<string,string>> cost_to_nodes;

	SearchableMatrix* matrix ;
public:
	AlgorithmTester(char* _path){
		std::vector<std::string> string_matrix;
		std::vector<std::string> row_elements;
		MatrixVec matrix_val;
		FileReader commandReader(_path);
		int PathSize;
		std::vector<std::string> path;

		string_matrix = commandReader.readByLine();

		for (std::string &line : string_matrix) {
			row_elements = LexerFunction::lexer(line);
			matrix_val.push_back(StringVecConverter(row_elements));
		}

		//get initial and goal point
		Point goal = extractPoint(matrix_val);
		Point initial = extractPoint(matrix_val);

		//set up a searchable matrix ;
		Matrix basic_matrix = Matrix(matrix_val);
		matrix = new SearchableMatrix(basic_matrix, initial, goal);

	}

	std::vector<double>
	StringVecConverter(std::vector<std::string> &stringVec) {
		std::vector<double> row;
		for (std::string &str : stringVec) {
			double val = stoi(str);
			row.push_back(val);
		}
		return row;
	}


	Point extractPoint(MatrixVec &data) {
		auto it = data.end() - 1;

		if (it->size() != POINT_SIZE) {
			throw "Illegal Point";
		}
		//must get a matrix and 2 points
		if (it == data.begin()) {
			throw "User did not enter an entry/exit points , or a matrix";
		}
		Point p = Point(it->at(X_POS), it->at(Y_POS));
		data.erase(it);
		return p;
	}

	std::string getPath(std::vector<Point> &solution,State<Point> * initial){
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
		return getPath(point_vec,initial);
	}

	double getCost(std::vector<Point> &solution,State<Point> * initial){
		double cost =0;
		for(Point p : solution){
			cost+=matrix->getCost(p);
		}
		return cost;
	}

	double getCost(std::vector<State<Point> *>& solution,State<Point> * initial){
		std::vector<Point> point_vec;
		if(solution.empty()){
			return -1;
		}
		const State<Point> * cur_state = solution[0];
		while(*cur_state!= *initial){
			point_vec.push_back(cur_state->getState());
			cur_state = cur_state->getParent();
		}
		point_vec.push_back(cur_state->getState());
		std::reverse(point_vec.begin() , point_vec.end());
		return getCost(point_vec,initial);
	}

//	std::string PathFromPoints (std::vector<Point> &solution) {
//		std::string path;
//		Point parent;
//		if(solution.empty()){
//			return path;
//		}
//		parent = solution[0] ;
//		int size = solution.size();
//		for(int i=1 ; i< size; i++){
//			Point cur_point = solution[i];
//			//x position is smaller
//			if(parent.first > cur_point.first){
//				path+="Up";
//			}else if(parent.first <cur_point.first){ //x position is larger
//				path+="Down";
//			}else if(parent.second > cur_point.second){	//y position is smaller
//				path+="Left";
//			}else if(parent.second < cur_point.second){//y position is larger
//				path+="Right";
//			}
//			if(i != size -1){
//				path+= ",";
//			}
//			parent = cur_point;
//		}
//		return path;
//	}
//
//	double printPath(std::vector<Point>& solution){
//		for (auto p:solution) {
//			std::string string_point = "(" + to_string(p.first) + "," +
//									   to_string(p.second) + ")" + "," ;
//			std::cout << string_point ;//<< std::endl;
//		}
//		std::cout << std::endl;
//		//TODO TESTER
//		std::cout << "Visual Route :" << std::endl;
//		std::string route = PathFromPoints(solution);
//		std::cout << route << std::endl ;
//
//		double cost=0 ;
//		for(Point p : solution){
//			cost+=matrix->getCost(p);
//		}
//		return cost;
//	}
//
//	double printPath(std::vector<State<Point> *>& solution){
//		std::vector<Point> point_vec;
//		if(solution.empty()){
//			return -1;
//		}
//		const State<Point> * cur_state = solution[0];
//		State<Point> * initial = matrix->getInitialState();
//		while(*cur_state!= *initial){
//			point_vec.push_back(cur_state->getState());
//			cur_state = cur_state->getParent();
//		}
//		point_vec.push_back(cur_state->getState());
//		std::reverse(point_vec.begin() , point_vec.end());
//		return printPath(point_vec);
//	}


	void saveMapToFile(){
		std::ofstream outfile;

		outfile.open(FILENAME, std::ios_base::app);
		for (auto i=this->cost_to_nodes.begin();
		i!=cost_to_nodes.end(); ++i){
			string cost = (*i).first;
			string nodes = (*i).second;
			outfile<<cost<<",";
			outfile<<nodes<<"\n";
		}
	}


	void handleClient(MatrixSearcher* searcher) {


		auto initial = matrix->getInitialState();
		auto solution = searcher->search(matrix);
		std::cout << "Solution : " << getPath(solution,initial) << std::endl;

		std::cout << "Cost : " << getCost(solution,initial) << std::endl;

		std::cout << "Evaluated Nodes: " << searcher->getNumberOfNodesEvaluated() << std::endl;


		string cost = to_string(getCost(solution,initial));
		string nodes = to_string(searcher->getNumberOfNodesEvaluated());

		this->cost_to_nodes.push_back(pair<string,string>(cost,nodes));
		saveMapToFile();

//		double cost = printPath(solution);


//		std::cout << std::endl;


		delete(matrix);

	}

	~AlgorithmTester(){
	}
};
#endif //SERVER_ALGORITHMTESTER_H