//
// Created by tomer on 1/11/19.
//

#ifndef SERVER_SOCKETS_H
#define SERVER_SOCKETS_H


#include <system_error>
#include <sys/socket.h>
#include <bits/socket.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "CommandReader/CommandReader.h"

namespace posix_sockets {

	// struct defining general socket, with general operations (open and close)
	// opening is done in the constructor.
	// the socket is not automatically closed in the destructor so that the object
	// can be passed as a parameter (possibly to another thread)
	struct TCP_socket {
		int sock_fd;

		TCP_socket() {
			sock_fd = socket(AF_INET, SOCK_STREAM, 0);
			if (sock_fd < 0) {
				throw std::system_error(
						std::error_code(errno, std::generic_category()),
						"failure on opening socket");
			}
		}

		TCP_socket(int open_sock_fd) {
			this->sock_fd = open_sock_fd;
		}

		void close() ;

		void settimeout(int sec, int usec = 0 );
	};

	class TCP_client {
		TCP_socket sock;
//		CommandReader* commandReader;

	public:
		// It makes sense to creates another constructor that
		// will create a client from scratch

		TCP_client(TCP_socket sock) : sock(sock) {}

//		// you should definitely use your own logic here
//		// suggestions are - read_until (char), read_min(int)
//		// read_line, etc.
//		// you can obviously define a write (or send) method
//		std::string read(int n);

		void settimeout(int sec, int usec = 0 );

		TCP_socket getSocket(){
			return sock;
		}

		void close();
	};

	class TCP_server {
		TCP_socket sock;
	public:
		explicit TCP_server()  {}

		TCP_server(int port) {
			sockaddr_in addr_in;
			addr_in.sin_family = AF_INET;
			addr_in.sin_port = htons(port);
			addr_in.sin_addr.s_addr = INADDR_ANY;

			if (bind(sock.sock_fd, (sockaddr *) &addr_in, sizeof(addr_in)) ==
				-1) {
				throw std::system_error(
						std::error_code(errno, std::generic_category()),
						"failure on bind");
			}
		}

		void listen(int max_lis);

		void settimeout(int sec, int usec  = 0);

		TCP_client accept();

		void close();
	};
}

#endif //SERVER_SOCKETS_H
