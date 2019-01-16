//
// Created by tomer on 1/9/19.
//

#ifndef SERVER_COMMANDLINEREADER_H
#define SERVER_COMMANDLINEREADER_H

#include "CommandReader.h"

class CommandLineReader : public CommandReader{
	int _socket;

public:

	CommandLineReader(int socket){
		this->_socket = socket;
	}

	virtual std::string read();

};
#endif //SERVER_COMMANDLINEREADER_H
