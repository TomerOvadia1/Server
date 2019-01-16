//
// Created by sagy on 1/6/19.
//

#ifndef SERVER_SERVER_SIDE_H
#define SERVER_SERVER_SIDE_H

#include "ClientHandle/ClientHandler.h"
#include "Sockets.h"

namespace server_side{
	/**
	 * Server Interface
	 */
    class Server {
        virtual void open(uint16_t port, ClientHandler* ch) = 0;
        virtual void stop() = 0;
//		virtual void* start(void*) = 0;

	};

	struct server_parameters {
		posix_sockets::TCP_client sock ;
		ClientHandler* clientHandler ;
	};

}


#endif //SERVER_SERVER_SIDE_H
