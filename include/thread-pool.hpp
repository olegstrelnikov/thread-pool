/*
 * thread-pool.hpp
 *
 *  Created on: Mar 9, 2017
 *      Author: ois
 */

#ifndef INCLUDE_THREAD_POOL_HPP_
#define INCLUDE_THREAD_POOL_HPP_

#include <atomic>
#include <mutex>
#include <list>

class ThreadPool {
public:
	ThreadPool(std::size_t threadNumber) {

	}
	typedef void (*task_t)();
	void addTask(task_t task) {

	}
private:
	static void worker_() {

	}
	std::list<task_t> tasks_;
};


#endif /* INCLUDE_THREAD_POOL_HPP_ */
