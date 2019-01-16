cmake_minimum_required(VERSION 3.13)
project(Server)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -pthread)

include_directories(.)
include_directories(CacheManager)
include_directories(CommandReader)
include_directories(Searchable)
include_directories(Searchers)
include_directories(Servers)
include_directories(Servers/ClientHandle)

add_executable(Server
		CacheManager/CacheManager.h
		CacheManager/FileCacheManager.cpp
		CacheManager/FileCacheManager.h
		CommandReader/CommandLineReader.cpp
		CommandReader/CommandLineReader.h
		CommandReader/CommandReader.h
		CommandReader/FileReader.h
		Searchable/ISearchable.h
		Searchable/Matrix.h
		Searchable/State.h
		Searchers/AStar.h
		Searchers/BestFirstSearch.h
		Searchers/BFS.h
		Searchers/DFS.h
		Searchers/ISearcher.h
		Servers/ClientHandle/ClientHandler.h
		Servers/ClientHandle/LexerFunction.cpp
		Servers/ClientHandle/LexerFunction.h
		Servers/ClientHandle/MatrixClientHandler.h
		Servers/ClientHandle/TestClientHandler.cpp
		Servers/ClientHandle/TestClientHandler.h
		Servers/Exceptions.h
		Servers/ParallelServer.cpp
		Servers/ParallelServer.h
		Servers/SerialServer.cpp
		Servers/SerialServer.h
		Servers/Server.h
		Servers/Sockets.cpp
		Servers/Sockets.h
		Servers/StartDecorator.h
		Servers/Task.h
		Servers/ThreadPool.h
		Servers/ThreadPoolTest.h
		AlgorithmTester.h
		main.cpp
		PriorityQueue.cpp
		PriorityQueue.h
		SearchingSolvers.h
		Solver.h
		StringReverser.h)
