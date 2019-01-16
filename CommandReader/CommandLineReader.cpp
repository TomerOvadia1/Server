//
// Created by tomer on 1/9/19.
//


#include <string>
#include "CommandLineReader.h"
#include <unistd.h>
#include <vector>
#include <iostream>


std::string CommandLineReader::read(){
	char curr_char;
	std::string line;
	std::vector<std::string> command_lines;

	do{
		ssize_t n= ::read(this->_socket, &curr_char, 1);
		//----
//		ssize_t n  = 1;
//		std::cin >> curr_char ;
		//------
		if(n<0){
			perror("Error reading from socket");
			exit(1);
		}
		line+=curr_char;

	}while(curr_char != '\n');
	return line;
	//line.find(TERMINATOR)==std::string::npos
}
