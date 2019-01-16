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


void test(int argc , char** argv){

    for (int i=1; i<argc; i++){
        char* path = argv[i];

        using Point = std::pair<int,int>;

        //Best First Search
        try{
            auto bestfs = new BestFirstSearch<Point>();
            auto tester = AlgorithmTester<vector<State<Point> *>>(path);
            tester.handleClient(bestfs);
        }catch(char const* s){
            std::cout << s << std::endl;
        }

        //DFS
        auto bfs_dfs_tester = AlgorithmTester< vector<Point>>(path);
        try{
            ISearcher<Point, vector<Point>>* dfs = new DFS<Point>();
            bfs_dfs_tester.handleClient(dfs);
        }catch(char const* s){
            std::cout << s << std::endl;
        }

        // BFS
        try{
            ISearcher<Point, vector<Point>>* bfs = new BFS<Point>();
            bfs_dfs_tester.handleClient(bfs);
        }catch(char const* s){
            std::cout << s << std::endl;
        }

        //AStar
        try{
            auto astar = new AStar<Point>();
            auto tester = AlgorithmTester<vector<State<Point> *>>(path);
            tester.handleClient(astar);
        }catch(char const* s){
            std::cout << s << std::endl;
        }
    }
}

int main(int argc , char** argv) {
    try{
        boot::ParallelMain::main(argc,argv);

    }catch (char* error){
        std::cout << error << std::endl;
    }



}