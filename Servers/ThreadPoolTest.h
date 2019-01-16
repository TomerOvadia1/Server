//
// Created by tomer on 1/13/19.
//

#ifndef SERVER_THREADPOOL_H
#define SERVER_THREADPOOL_H


#include <vector>
#include <pthread.h>
#include <condition_variable>
#include <functional>
#include <queue>
#include <iostream>
#include <thread>


class ThreadPoolTest {
public:
	//task to run , no return type is specified at this moment
	using Task = std::function<void()>;
	// ^ - if we want to use a return value we can use future - dont know how
	// to use TODO READ IF NEEDED


	explicit ThreadPoolTest(std::size_t numThreads){
		start(numThreads);


	}

	~ThreadPoolTest(){
		stop();
	}

	//get the current task
	void enqueue(Task task){
		//using a scope inside method scope is used to restrict the scope of
		// logs for the MUTEX !
		{
			std::unique_lock<std::mutex> lock{mEventMutex};
			//move is - moving the original value to another place by reference
			//like moving the original to another place using it since it is
			// about to be
			//i think it is moving it by reference - means i used to avoid
			// copies
			mTasks.emplace(std::move(task));
		}

		mEventVar.notify_one();
	}

private:
	std::vector<std::thread> mThreads;

	//announce event happened
	std::condition_variable mEventVar;

	//mutex for parallel work on threads
	std::mutex mEventMutex ;
	//tells a thread to stop
	bool mStopping = false;

	std::queue<Task> mTasks;


	void start(std::size_t numThreads)
	{
		//loop on threads
		for(auto i=0 ; i<numThreads ; i++)
		{
			//add a thread with given function
			//lambda capture "=" - means capture by copy of this ? i think
			mThreads.emplace_back([=] {
				while(true)
				{
					Task task;
					//we use another scope to keep the critical section as
					// small as possible and dont want the mutex to be locked
					// while the task is executing , since task might take
					// a lot of time (we dont know that ), and the mutex
					// might then be locked for a long time which will block
					// other threads - which is bad
					{
						//acquire a unique lock - done for writing
						//not like a shared lock which 2 threads can use at once
						// - reading usually
						std::unique_lock<std::mutex> lock{mEventMutex};
						//function wait until the condition variable is announced .
						//then , it runs the lambda function
						//stops if we are either telling to stop of queue is
						// not empty meaning there is a task to be done
						mEventVar.wait(lock,
								[=] { return mStopping ||!mTasks.empty(); });

						//if we need to stop AND task list is empty meaning
						// all tasks has completed
						if (mStopping && mTasks.empty()) {
							break;
						}

						//take the first task from queue
						task = std::move(mTasks.front());
						//pop it
						mTasks.pop();
					}
					//run task
					task();

				}

			});
		}
	}

	void stop(){
		{
			std::unique_lock<std::mutex> lock{mEventMutex};
			mStopping = true;
		}

		//notify all conditional variables
		mEventVar.notify_all();

		for(auto &thread : mThreads){
			//wait for threads to end work
			//might use thread.join()
			thread.join();
		}
	}
};

int driverProg(){
	{
		//testing
		ThreadPoolTest pool{5};

		pool.enqueue([] {
			std::cout << "1" << std::endl;

		});

		pool.enqueue([] {
			std::cout << "2" << std::endl;
		});

		pool.enqueue([] {
			std::cout << "3" << std::endl;
		});

		pool.enqueue([] {
			std::cout << "4" << std::endl;
		});

		pool.enqueue([] {
			std::cout << "5" << std::endl;
		});
	}


	return 0;


}
#endif //SERVER_THREADPOOL_H
