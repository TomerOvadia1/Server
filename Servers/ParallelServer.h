//
// Created by sagy on 1/7/19.
//

#ifndef SERVER_MYPARALLELSERVER_H
#define SERVER_MYPARALLELSERVER_H
//#include "Sockets.h"
#include "Server.h"
//#include "ThreadPool.h"
//#include "SerialServer.h"

namespace boot {

	class ParallelServer : public server_side::Server {
		posix_sockets::TCP_server server ;

	public:
		ParallelServer() {}

		virtual void open(uint16_t port, ClientHandler *ch);

		virtual void stop() {
			server.close();
		}

	};


	class ParallelMain{
	public:
		static int main(int argc , char** argv);
	};




}


#endif //SERVER_MYPARALLELSERVER_H
