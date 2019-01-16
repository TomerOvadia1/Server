//
// Created by tomer on 1/15/19.
//

#ifndef SERVER_FILEREADER_H
#define SERVER_FILEREADER_H

#include "CommandReader.h"
#include <fstream>
#include <string>
#include <vector>

/**
 * file reader class
 */
class FileReader : public CommandReader {

	std::fstream _file;
public:
	FileReader(char* path){
		this->_file.open(path, std::fstream::in);
		if (!this->_file.is_open()) {
			if (!this->_file.is_open()){
				throw "Error:cannot open given file" ;
			}
		}
	}

	virtual std::string read(){
		std::string line ;
		std::string commands;
		//read line
		while(getline(this->_file, line)){
			//if getline reads EOF return nullptr
			commands += line + "\n";
		}
		_file.close();
		return commands;
	}

	std::vector<std::string> readByLine(){
		std::string line ;
		std::vector<std::string> commands;
		//read line
		while(getline(this->_file, line)){
			if(!line.empty() && line!="\n"){
				commands.push_back(line);
			}
		}
		_file.close();
		return commands;
	}
//	virtual std::string read_scope();

};

#endif //SERVER_FILEREADER_H
