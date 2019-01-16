// if you use this code in your project, remember to use
// #ifndef, #define, #endif
// also, make sure to separate implementation (.cpp) from header (.h)

// it is also possible and recommended to seperate the exceptions
// declarations to a separate file

// you need: MyExceptions.h, Sockets.h, Sockets.cpp

// of course, remove the main() function

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>
#include <string>
#include <stdexcept>
#include "Sockets.h"
#include "Exceptions.h"

namespace posix_sockets {

	/*
 	* TCP_socket
 	*/

	void TCP_socket::close() {
		if (::close(sock_fd) < 0) {
			throw std::system_error(
					std::error_code(errno, std::generic_category()),
					"failure on closing socket");
		}
	}

	void TCP_socket::settimeout(int sec, int usec) {
		timeval timeout;
		timeout.tv_sec = sec;
		timeout.tv_usec = usec;

		// setting socket option for recieve timeout
		if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO,
					   (char *) &timeout, sizeof(timeout)) == -1) {
			throw std::system_error(
					std::error_code(errno, std::generic_category()),
					"failure on setsockopt");
		}
	}

	/*
	 * TCP_client
	 */

	void TCP_client::settimeout(int sec, int usec ) {
		sock.settimeout(sec, usec);
	}

//	std::string TCP_client::read(int n) {
//		char *buffer = new char[n + 1];
//		int read_len = ::read(sock.sock_fd, buffer, n);
//		if (read_len < 0) {
//			if (errno == EAGAIN || errno == EWOULDBLOCK) {
//				throw timeout_exception("timeout on read");
//			}
//			throw std::system_error(
//					std::error_code(errno, std::generic_category()),
//					"error on read");
//		}
//
//		buffer[n] = 0;
//		std::string output = std::string(buffer);
//		delete buffer;
//		return output;
//	}

	void TCP_client::close() {
		sock.close();
	}

	/*
	 * TCP_server
	 */


	TCP_client TCP_server::accept() {
		sockaddr_in addr;
		socklen_t len = sizeof(addr);
		int client_sock_fd = ::accept(sock.sock_fd, (sockaddr *) &addr, &len);
		if (client_sock_fd < 0) {
			// eagain and ewouldblock are errors normally hapenning on timeouts
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				throw timeout_exception("timeout on accept");
			} else {
				throw std::system_error(
						std::error_code(errno, std::generic_category()),
						"error on accept");
			}
		}

		TCP_socket client_sock(client_sock_fd);

		// in Unix, client socket have their parents' timeout
		// so we set the timeout to 0 (effectively - infinity)
		// for newly created sockets
		client_sock.settimeout(0);
		return client_sock;
	}

	void TCP_server::listen(int max_lis) {
		if (::listen(sock.sock_fd, max_lis) == -1) {
			throw std::system_error(
					std::error_code(errno, std::generic_category()),
					"error on listen");
		}
	}
	void TCP_server::settimeout(int sec, int usec ) {
		sock.settimeout(sec, usec);
	}

	void TCP_server::close() {
		sock.close();
	}
}

