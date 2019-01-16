//
// Created by sagy on 1/6/19.
//

#ifndef SERVER_FILECACHEMANAGER_H
#define SERVER_FILECACHEMANAGER_H


#include "CacheManager.h"
#include <map>
#include <fstream>
#include <iostream>
#include <mutex>

#define FILE_NAME "problems_solutions.txt"

class FileCacheManager : public CacheManager<std::string, std::string> {
    std::map <std::string, std::string> problems_solutions;
    std::mutex m ;

public:
    FileCacheManager(){
        this->problems_solutions.clear();
        std::string line;
        std::string prob;
        std::string sol;
        std::ifstream myfile (FILE_NAME);
        if (myfile.is_open()){

            while(getline (myfile,line)){
                if(line.empty()){
                    while(getline (myfile,line)){
                        if (line.empty()){
                            break;
                        }
                        sol += line + "\n";
                    }
                    this->update_map(prob, sol);
                }
                prob += line + "\n";
            }

            myfile.close();
        }
    }

    virtual bool is_solution_exists(std::string prob) {
        std::lock_guard<std::mutex> lock{m};
        auto it = problems_solutions.find(prob);
        return it!=this->problems_solutions.end();
    }

    virtual std::string getSolution(std::string prob) {
        std::lock_guard<std::mutex> lock{m};
        return problems_solutions.at(prob);
    }

    virtual void saveMapToFile();

    void update_map(std::string prob, std::string sol){
        std::lock_guard<std::mutex> lock{m};
        this->problems_solutions.insert(std::pair
        <std::string,std::string>(prob,sol));
    }

    void loadFileToMap();

    std::string readUntilNewLine(std::ifstream &myfile);
};


#endif //SERVER_FILECACHEMANAGER_H
