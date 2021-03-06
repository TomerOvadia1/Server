//
// Created by tomer on 1/14/19.
//

#ifndef SERVER_THREADPOOL_H
#define SERVER_THREADPOOL_H

#include <thread>
#include <queue>
#include <atomic>
#include <mutex>
//#include <string>
//#include <iostream>
#include <condition_variable>		// to avoid busy waiting
#include "Task.h"



class TasksQueue
{
	std::atomic<bool> stop_queue;
	std::queue<Task*> tasks;
	mutable std::condition_variable queue_cond_var;
	mutable std::mutex mut;

public:
	TasksQueue() {
		stop_queue = false;
	}
	Task* pop() {
		std::lock_guard<std::mutex> g(mut);
		if (tasks.empty()) {
			return nullptr;
		}
		auto task = tasks.front();
		tasks.pop();
		return task;
	}

	bool empty() const {
		std::lock_guard<std::mutex> g(mut);
		return tasks.empty();
	}

	void push(Task* task) {
		if (task == nullptr) {
			throw std::invalid_argument("Task should not be null");
		}
		std::lock_guard<std::mutex> g(mut);
		tasks.push(task);
		queue_cond_var.notify_one();
	}

	void wait() const {
		std::unique_lock<std::mutex> ul(mut);
		if (stop_queue) return;
		queue_cond_var.wait(ul);
	}

	bool stop() const {
		return stop_queue;
	}

	void exit() {
		std::lock_guard<std::mutex> g(mut);
		stop_queue = true;
		queue_cond_var.notify_all();
	}
};

void worker(TasksQueue* queue)
{
	//&& !queue->empty()
	while (true) {
		queue->wait();
		if(queue->stop() && queue->empty()){
			break;
		}
		Task* task = queue->pop();
		if (task) {
			task->execute();
			delete task;
		}

	}
}

#endif //SERVER_THREADPOOL_H
