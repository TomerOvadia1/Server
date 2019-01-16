#include "AlgorithmTester.h"
#include "Searchers/BFS.h"
#include "Searchers/DFS.h"
#include "Searchers/BestFirstSearch.h"
#include <vector>
#include <CacheManager/FileCacheManager.h>
#include "SearchingSolvers.h"
#include "Servers/ParallelServer.h"
#include "LexerFunction.h"
#include <string>
#include <algorithm>
#include <Searchers/AStar.h>

//const int PATH_ENTERED = 2;
//const int PORT_POS = 1;
using MatrixVec = std::vector<std::vector<double>>;


std::vector<double> StringVecConverter(std::vector<std::string>& stringVec){
    std::vector<double> row ;
    for(std::string& str : stringVec){
        double val = stoi(str);
        row.push_back(val);
    }
    return row;
}

int main(int argc , char** argv) {

	using Point = std::pair<int,int>;
//	using namespace std;
//	char* path = argv[1];

	// BFS OR DFS
//	try{
//		ISearcher<Point, vector<Point>>* bfs = new BFS<Point>();
//		auto tester = AlgorithmTester< vector<Point>>(argv[1]);
//		tester.handleClient(bfs);
//	}catch(char const* s){
//		std::cout << s << std::endl;
//	}

//
//	// ASTAR OR BEST FIRST SEARCH
//  	ISearcher<Point, vector<State<Point> *>>* bfs = new AStar<Point>();
//	auto tester = AlgorithmTester<vector<State<Point> *>>(argv[1]);
//	tester.handleClient(bfs);



//	tester.handleClient(bfs);

//    int port =0;
//    auto cmd = new CommandLineReader(port);
//
//    std::string line = cmd->CommandLineReader::read();
//    std::cout << line << endl;
////    delete commandReader;
    //fcm.

//    std::cout << dfs->getNumberOfNodesEvaluated() << std::endl;
//    std::cout << "done" <<std::endl;


//    std::string problem = "1  , 2   ,-1 , 3 \n";
//    vector<string> input = LexerFunction::lexer(problem);
//    std::vector<std::vector<double>> x;
//
//    std::vector<double> y;
//    y.push_back(1);
//    y.push_back(2);
//    y.push_back(3);
//    x.push_back(y);
//    y.clear();
//
//    y.push_back(4);
//    y.push_back(5);
//    y.push_back(6);
//    x.push_back(y);
//    y.clear();
//
//    y.push_back(8);
//    y.push_back(9);
//    y.push_back(10);
//    x.push_back(y);
//
//    Matrix m(x) ;
//    Matrix::Point init(0,0);
//    Matrix::Point exit(2,2);
//
//    SearchableMatrix searchableMatrix(m,init,exit);
//    DFS<Matrix::Point> dfs;
//    dfs.search(&searchableMatrix);
//
//    std::cout << dfs.getNumberOfNodesEvaluated() << endl;
//    std::cout << "done" <<std::endl;

//    driverProg();
    boot::ParallelMain::main(argc,argv);


//    std::vector<std::string> row_elements;
//    std::vector<std::string> row;
//    MatrixVec matrix_val;
//    int PathSize;
//    std::vector<std::string> path;
//
//    row = {"4, 2, 9,2,6 \n",
//                  "4, 0,10,3,7 \n",
//                  "4, 5, 2,5,8 \n",
//                  "1, 2, 3,4 ,9 \n",
//                  "4, 5, 3,9 ,8 \n",
//                  "0,0\n",
//                  "4,4\n",
//                  "end"};
//
//    //loop while user did not enter a terminator
//    int i=0;
//    while (row[i].find("end") == std::string::npos) {
//        std::cout << "Loop: " << std::endl;
//
//        row_elements = LexerFunction::lexer(row[i]);
//        matrix_val.push_back(StringVecConverter(row_elements));
//        i++;
//    }
//    //line contains
//    row[i].substr(0,row.size()-row[i].find("end"));
//    if(!row.empty()){
//        row_elements = LexerFunction::lexer(row[i]);
//        matrix_val.push_back(StringVecConverter(row_elements));
//    }
//    i++;

}