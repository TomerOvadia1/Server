//
// Created by tomer on 1/14/19.
//

#ifndef SERVER_STARTDECORATOR_H
#define SERVER_STARTDECORATOR_H
#include "Task.h"
#include "Servers/Server.h"

class StartDecorator : public Task
{
	using  server_parameters = server_side::server_parameters;

	void* serverParams;
public:
	StartDecorator(void* params) {
		serverParams = params;
	}

	void execute() {
		auto p = (server_parameters*)serverParams;
		server_parameters serverParameters = {p->sock , p->clientHandler} ;
		delete(p);
		int port = serverParameters.sock.getSocket().sock_fd;
		serverParameters.clientHandler->handleClient(port);
		//TODO ---- ADDED THIS , CHECK IF ITS WORKING
		serverParameters.sock.close();
	}
};

#endif //SERVER_STARTDECORATOR_H
