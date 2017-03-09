/*
 * thread-pool.hpp
 *
 *  Created on: Mar 9, 2017
 *      Author: ois
 */

#ifndef INCLUDE_THREAD_POOL_HPP_
#define INCLUDE_THREAD_POOL_HPP_

#include <mutex>
#include <list>
#include <thread>

class ThreadPool {
public:
	ThreadPool(std::size_t threadNumber) {
		for (; threadNumber; --threadNumber) {
			std::thread(&ThreadPool::worker_, this).detach();
		}
	}
	typedef void (*task_t)();
	void addTask(task_t task) {
		std::lock_guard<std::mutex> guard(m_);
		tasks_.push_back(task);
	}
	~ThreadPool() {
		while (true) {
			std::lock_guard<std::mutex> guard(m_);
			if (tasks_.empty()) {
				break;
			}
		}
	}
private:
	void worker_() {
		task_t task = nullptr;
		while (true) {
			{
				std::lock_guard<std::mutex> guard(m_);
				if (!tasks_.empty()) {
					task = tasks_.front();
					tasks_.erase(tasks_.begin());
				}
			}
			if (task) {
				task();
				task = nullptr;
			}
		}
	}
	std::list<task_t> tasks_;
	std::mutex m_;
};

#endif /* INCLUDE_THREAD_POOL_HPP_ */
