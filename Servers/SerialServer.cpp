//
// Created by sagy on 1/6/19.
//

#include "SerialServer.h"
#include "Servers/Sockets.h"
#include "StringReverser.h"
#include "Servers/ClientHandle/TestClientHandler.h"
#include "Servers/ClientHandle/ClientHandler.h"
#include <string>
#include "Servers/Exceptions.h"

const int TIMEOUT_SEC = 10;
const int PORT_ENTERED = 2;
const int PORT_POS = 1;


int boot::SerialMain::main(int argc , char** argv) {

	int portno;
	ClientHandler* clientHandler;
	StringReverser* reverser = new StringReverser();

	if(argc !=PORT_ENTERED){
		throw "Program must except port number as second argument";
	}
	//get port from program arguments
	portno = std::stoi(argv[PORT_POS]);
	//set client handler as string reverser
	clientHandler = new TestClientHandler<std::string,std::string>
	        (reverser);
	//set a new serial server
	boot::SerialServer server;
	//open server on given port and listen to incoming requests
	server.open(portno,clientHandler);
}


void boot::SerialServer::open(uint16_t portno, ClientHandler* clientHandler) {

	using namespace posix_sockets;
	pthread_t pthread;
	//set up a new server
	TCP_server server(portno);
	server.listen(SOMAXCONN);
	TCP_client client = server.accept();
	/* init server_parameters struct */
	auto serverParameters =
			new server_side::server_parameters{client ,clientHandler };

	//create a new pthread  , reading data from server
	pthread_create(&pthread, nullptr, start, serverParameters);
	pthread_join(pthread,nullptr);
	server.settimeout(TIMEOUT_SEC);
	try{
		while (true) {
			/* Accept actual connection from the client */
			client = server.accept();

			/* init server_paramaters struct */
			auto serverParameters =
					new server_side::server_parameters{client ,clientHandler };

			//create a new pthread  , reading data from server
			pthread_create(&pthread, nullptr, start, serverParameters);
			pthread_join(pthread,nullptr);
		}
	}catch(timeout_exception& exception1){
		//tell client connection stopped due to timeout
		std::cout << exception1.what() << std::endl;

	}
	/* catch(...)  will be thrown */

	//close socket
	server.close();

}



//
//	pthread_t pthread;
//
//	int sockfd, newsockfd, clilen;
//	struct sockaddr_in serv_addr, cli_addr;
//
//	/* First call to socket() function */
//	sockfd = socket(AF_INET, SOCK_STREAM, 0);
//	if (sockfd < 0) {
//		perror("Error : opening socket");
//		exit(1);
//	}
//	/* Initialize socket structure */
////	bzero((char *) &serv_addr, sizeof(serv_addr));
//
//	//check if port is valid
//	if (portno > UINT16_MAX) {
//		throw "Port must be in range  0- 65535 ";
//	}
//
//	serv_addr.sin_family = AF_INET;
//	serv_addr.sin_addr.s_addr = INADDR_ANY;
//	serv_addr.sin_port = htons(portno);
//
//	/* Now bind the host address using bind() call.*/
//	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
//		perror("ERROR on binding");
//		exit(1);
//	}
//
//	/* Now start listening for the clients, here process will
//	   * go in sleep mode and will wait for the incoming connection
//	*/
//
//	listen(sockfd, SO_MAX_CONN);
//	clilen = sizeof(cli_addr);
//
//	while(true) {
//
//		timeval timeout;
//		timeout.tv_sec = TIMEOUT_SEC;
//		timeout.tv_usec = TIMEOUT_MICROSEC;
//		setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout,
//				   sizeof(timeout));
//
//		/* Accept actual connection from the client */
//		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
//						   (socklen_t *) &clilen);
//
//		if (newsockfd < 0) {
//			if (errno == EWOULDBLOCK) {
//				break;
//
//			} else {
//				perror("Error occurred while accepting connection");
//				exit(2);
//			}
//		}
//
//		/* init server_paramaters struct */
//		auto serverParameters =
//				new server_parameters{portno, newsockfd};
//
//		//create a new pthread  , reading data from server
////		pthread_create(&pthread, nullptr, clientHandler->handleClient,
////					   serverParameters);
//	}


