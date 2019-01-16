//
// Created by tomer on 1/9/19.
//

#ifndef SERVER_COMMANDREADER_H
#define SERVER_COMMANDREADER_H

#include <string>

class CommandReader {

public:

	virtual std::string read() = 0;

//	virtual ~CommandReader(){}

};

#endif //SERVER_COMMANDREADER_H
