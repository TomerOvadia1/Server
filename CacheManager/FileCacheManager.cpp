//
// Created by sagy on 1/6/19.
//

#include "FileCacheManager.h"

/**
 * save all map to file
 */
void FileCacheManager::saveMapToFile(){
    std::ofstream myfile (FILE_NAME);

    if (myfile.is_open() , std::ofstream::trunc) {

        for(auto& p : problems_solutions){
            myfile << p.first + "\n" ;
            myfile << "$\n" ;
            myfile << p.second + "\n" ;
            myfile << "#\n" ;
        }

        myfile.close();
    }
    else std::cout << "Unable to save to file";
}


/*
 * std::map<std::string, std::string>::iterator it;

        for ( it = this->problems_solutions.begin();
              it != this->problems_solutions.end(); it++ ){
            std::string prob = it->first;
            std::string sol = it->second;
            myfile << prob << "\n";
            myfile << sol << "\n";
            myfile << "\n";
        }
 */