//
// Created by sagy on 1/7/19.
//

#include <Servers/ClientHandle/MatrixClientHandler.h>
#include <SearchingSolvers.h>
#include "ParallelServer.h"
#include "ThreadPool.h"
#include "Sockets.h"
#include "StartDecorator.h"
#include "Exceptions.h"
#include "Searchers/AStar.h"
#include "CacheManager/FileCacheManager.h"

const int TIMEOUT_SEC = 1;
const int PORT_ENTERED = 2;
const int PORT_POS = 1;
const int POOL_SIZE = 2;


int boot::ParallelMain::main(int argc , char** argv){

	using Point = std::pair<int,int>;
	using stateVec = std::vector<State<Point> *>;

	if(argc !=PORT_ENTERED){
		throw "Program must except port number as second argument";
	}


	int portno;
	ClientHandler* clientHandler;
	ISearcher<Point,stateVec >* searcher ;
	Solver<ISearchable<Point>* ,stateVec >* solver ;
	auto cacheManager = new FileCacheManager();

	cacheManager->loadFileToMap();

	//get port from program arguments
	portno = std::stoi(argv[PORT_POS]);

	searcher = new AStar<Point>();
	solver = new SearchingSolvers<Point,stateVec >(searcher);
	//set client handler as string reverser
	clientHandler = new MatrixClientHandler(solver,cacheManager);

	//set a new serial server
	boot::ParallelServer server;
	//open server on given port and listen to incoming requests
	server.open(portno,clientHandler);
	server.stop();

//	//save new solutions to file
	cacheManager->saveMapToFile();

	//delete allocated objects
	delete (searcher);
	delete (solver);
	delete (clientHandler);
	delete(cacheManager);
}


void boot::ParallelServer::open(uint16_t port, ClientHandler* clientHandler) {

	using namespace posix_sockets;
	using server_parameters = server_side::server_parameters;

	bool should_continue = true;
//	ThreadPoolTest pool{POOL_SIZE};
	//set up a new server
	server = TCP_server (port);
	server.listen(SOMAXCONN);
	std::queue<std::thread> workers;
	TasksQueue tasks_queue;

	//create a thread pool
	for (int i = 0; i < POOL_SIZE; ++i) {
		workers.push(std::thread(worker, &tasks_queue));
	}

	//accept first client
	TCP_client client = server.accept();
	auto serverParameters =
			new server_side::server_parameters{client ,clientHandler };
	tasks_queue.push(new StartDecorator(serverParameters));
	//set time out value
	server.settimeout(TIMEOUT_SEC);
	//loop until no more clients
	while (true) {
		try{
			client = server.accept();
			serverParameters =
					new server_side::server_parameters{client ,clientHandler };
			tasks_queue.push(new StartDecorator(serverParameters));
		}catch(timeout_exception& exception1){
			//connection stopped due to timeout
			tasks_queue.exit();
			break;
		}
	}

	//loop to execute remaining tasks
	while (!workers.empty()) {
		workers.front().join();
		workers.pop();
	}
}

