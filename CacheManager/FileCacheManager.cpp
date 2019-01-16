//
// Created by sagy on 1/6/19.
//

#include "FileCacheManager.h"

const char NEWLINE_FEED = '\n';

/**
 * save all map to file
 */
void FileCacheManager::saveMapToFile(){
    std::ofstream myfile (FILE_NAME);

    if (myfile.is_open()) {

        std::map<std::string, std::string>::iterator it;

        for ( it = this->problems_solutions.begin();
              it != this->problems_solutions.end(); it++ ){
            std::string prob = it->first;
            std::string sol = it->second;
            myfile << prob << "\n";
            myfile << sol << "\n";
            myfile << "\n";
        }

        myfile.close();
    }
    else std::cout << "Unable to save to file";
}


/**
 * load problems and solution from to and update map
 */
void FileCacheManager::loadFileToMap(){
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