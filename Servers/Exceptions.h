//
// Created by tomer on 1/11/19.
//

#ifndef SERVER_EXCEPTIONS_H
#define SERVER_EXCEPTIONS_H

#include <stdexcept>
namespace posix_sockets {

	class timeout_exception : public std::runtime_error {
	public:
		timeout_exception(const char *msg) : std::runtime_error(msg) {}

		timeout_exception(std::string msg) : std::runtime_error(msg) {}
	};

	class illegal_state_exception : public std::logic_error {
	public:
		illegal_state_exception(std::string msg) : std::logic_error(msg) {}
	};
}
#endif //SERVER_EXCEPTIONS_H
