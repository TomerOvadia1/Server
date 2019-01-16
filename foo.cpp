#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	cout << argv[0] << endl;
	if (argc < 2)	{
		cout << "Usage: <prog> <port>" << endl;
		return 1;
	}
	int port = atoi(argv[1]);
	int s = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serv;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port = htons(port);
	serv.sin_family = AF_INET;
	if (bind(s, (sockaddr *)&serv, sizeof(serv)) < 0)	{
		cerr << "Bad!" << endl;
	}

	int new_sock;
	listen(s, 5);
	struct sockaddr_in client;
	socklen_t clilen = sizeof(client);

	timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;

	setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));

	new_sock = accept(s, (struct sockaddr*)&client, &clilen);
	if (new_sock < 0)	{
		if (errno == EWOULDBLOCK)	{
			cout << "timeout!" << endl;
			exit(2);
		}	else	{
			perror("other error");
			exit(3);
		}
	}
	cout << new_sock << endl;
	cout << s << endl;
	close(new_sock);
	close(s);
	return 0;
}//
// Created by tomer on 1/9/19.
//


// if you use this code in your project, remember to use
// #ifndef, #define, #endif
// also, make sure to separate implementation (.cpp) from header (.h)

// it is also possible and recommended to seperate the exceptions
// declarations to a separate file

// you need: MyExceptions.h, Sockets.h, Sockets.cpp

// of course, remove the main() function

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <string>
#include <system_error>
#include <stdexcept>


namespace posix_sockets
{
	class timeout_exception : public std::runtime_error
	{
	public:
		timeout_exception(const char* msg) : std::runtime_error(msg){}
		timeout_exception(std::string msg) : std::runtime_error(msg){}
	};

	class illegal_state_exception : public std::logic_error
	{
	public:
		illegal_state_exception(std::string msg) : std::logic_error(msg){}
	};


	// struct defining general socket, with general operations (open and close)
	// opening is done in the constructor.
	// the socket is not automatically closed in the destructor so that the object
	// can be passed as a parameter (possibly to another thread)
	struct TCP_socket
	{
		int sock_fd;
		TCP_socket()
		{
			sock_fd = socket(AF_INET, SOCK_STREAM, 0);
			if (sock_fd < 0)	{
				throw std::system_error(std::error_code(errno, std::generic_category()), "failure on opening socket");
			}
		}

		TCP_socket(int open_sock_fd)
		{
			this->sock_fd = open_sock_fd;
		}

		void close()
		{
			if (::close(sock_fd) < 0)	{
				throw std::system_error(std::error_code(errno, std::generic_category()), "failure on closing socket");
			}
		}

		void settimeout(int sec, int usec = 0)
		{
			timeval timeout;
			timeout.tv_sec = sec;
			timeout.tv_usec = usec;

			// setting socket option for recieve timeout
			if (setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO,
						   (char *)&timeout, sizeof(timeout)) == -1)	{
				throw std::system_error(std::error_code(errno, std::generic_category()), "failure on setsockopt");
			}
		}
	};

	class TCP_client
	{
		TCP_socket sock;
	public:
		// It makes sense to creates another constructor that
		// will create a client from scratch

		TCP_client(TCP_socket sock) : sock(sock) {}

		// you should definitely use your own logic here
		// suggestions are - read_until (char), read_min(int)
		// read_line, etc.
		// you can obviously define a write (or send) method
		std::string read(int n)
		{
			char* buffer = new char[n + 1];
			int read_len = ::read(sock.sock_fd, buffer, n);
			if (read_len < 0)	{
				if (errno == EAGAIN || errno == EWOULDBLOCK)	{
					throw timeout_exception("timeout on read");
				}
				throw std::system_error(std::error_code(errno, std::generic_category()), "error on read");
			}

			buffer[n] = 0;
			std::string output = std::string(buffer);
			delete buffer;
			return output;
		}

		void settimeout(int sec, int usec = 0)
		{
			sock.settimeout(sec, usec);
		}

//		void close()
//		{
//			sock.close();
//		}
	};

	class TCP_server
	{
		TCP_socket sock;
	public:
		TCP_server(int port)
		{
			sockaddr_in addr_in;
			addr_in.sin_family = AF_INET;
			addr_in.sin_port = htons(port);
			addr_in.sin_addr.s_addr = INADDR_ANY;

			if (bind(sock.sock_fd, (sockaddr *)&addr_in, sizeof(addr_in)) == -1)	{
				throw std::system_error(std::error_code(errno,
						std::generic_category()), "failure on bind");
			}
		}

		void listen(int max_lis)
		{
			if (::listen(sock.sock_fd, max_lis) == -1)	{
				throw std::system_error(std::error_code(errno, std::generic_category()), "error on listen");
			}
		}

		void settimeout(int sec, int usec = 0)
		{
			sock.settimeout(sec, usec);
		}

		TCP_client accept()
		{
			sockaddr_in addr;
			socklen_t len = sizeof(addr);
			int client_sock_fd = ::accept(sock.sock_fd, (sockaddr*)&addr, &len);
			if (client_sock_fd < 0)	{
				// eagain and ewouldblock are errors normally hapenning on timeouts
				if (errno == EAGAIN || errno == EWOULDBLOCK)	{
					throw timeout_exception("timeout on accept");
				}	else	{
					throw std::system_error(std::error_code(errno, std::generic_category()), "error on accept");
				}
			}

			TCP_socket client_sock(client_sock_fd);

			// in Unix, client socket have their parents' timeout
			// so we set the timeout to 0 (effectively - infinity)
			// for newly created sockets
			client_sock.settimeout(0);
			return client_sock;
		}

		void close()
		{
			sock.close();
		}
	};
}

#include <condition_variable>
#include <functional>
#include <iostream>
#include <future>
#include <vector>
#include <thread>
#include <queue>

class ThreadPoolTest
{
public:
	using Task = std::function<void()>;

	explicit ThreadPoolTest(std::size_t numThreads)
	{
		start(numThreads);
	}

	~ThreadPoolTest()
	{
		stop();
	}

	template<class T>
	auto enqueue(T task)->std::future<decltype(task())>
	{
		auto wrapper = std::make_shared<std::packaged_task<decltype(task()) ()>>(std::move(task));

		{
			std::unique_lock<std::mutex> lock{mEventMutex};
			mTasks.emplace([=] {
				(*wrapper)();
			});
		}

		mEventVar.notify_one();
		return wrapper->get_future();
	}

private:
	std::vector<std::thread> mThreads;

	std::condition_variable mEventVar;

	std::mutex mEventMutex;
	bool mStopping = false;

	std::queue<Task> mTasks;

	void start(std::size_t numThreads)
	{
		for (auto i = 0u; i < numThreads; ++i)
		{
			mThreads.emplace_back([=] {
				while (true)
				{
					Task task;

					{
						std::unique_lock<std::mutex> lock{mEventMutex};

						mEventVar.wait(lock, [=] { return mStopping || !mTasks.empty(); });

						if (mStopping && mTasks.empty())
							break;

						task = std::move(mTasks.front());
						mTasks.pop();
					}

					task();
				}
			});
		}
	}

	void stop() noexcept
	{
		{
			std::unique_lock<std::mutex> lock{mEventMutex};
			mStopping = true;
		}

		mEventVar.notify_all();

		for (auto &thread : mThreads)
			thread.join();
	}
};

int driver()
{
	{
		ThreadPoolTest pool{36};

		for (auto i = 0; i < 36; ++i)
		{
			pool.enqueue([] {
				auto f = 1000000000;
				while (f > 1)
					f /= 1.00000001;
			});
		}
	}

	return 0;
}