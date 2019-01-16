//
// Created by sagy on 1/6/19.
//

#ifndef SERVER_MYSERIALSERVER_H
#define SERVER_MYSERIALSERVER_H

#include "Servers/Server.h"
#include "CommandReader/CommandReader.h"
#include "Servers/Sockets.h"


namespace boot {


	void* start(void* serverParams){
		auto p = (server_side::server_parameters*)serverParams;
		server_side::server_parameters serverParameters = {p->sock , p->clientHandler} ;
		delete(p);
		int port = serverParameters.sock.getSocket().sock_fd;
		serverParameters.clientHandler->handleClient(port);
		//TODO ---- ADDED THIS , CHECK IF ITS WORKING
		serverParameters.sock.close();
	}


	class SerialServer : public server_side::Server {

	public:
		SerialServer() {}

//		std::string read(){
//			return commandReader->read();
//		}

		virtual void open(uint16_t port, ClientHandler *ch);

//		virtual void* start(void* serverParams);

		virtual void stop() {}

	};

	class SerialMain{

	public:
		static int main(int argc , char** argv);
	};

}

#endif //SERVER_MYSERIALSERVER_H
