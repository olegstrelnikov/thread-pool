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
#include <vector>
#include <condition_variable>

class ThreadPool {
public:
	ThreadPool(std::size_t threadNumber, std::size_t usec = 1) : active_(true) {
		threads_.reserve(threadNumber);
		for (; threadNumber; --threadNumber) {
			threads_.emplace_back(&ThreadPool::worker_, this);
		}
	}
	typedef void (*task_t)();
	void addTask(task_t task) {
		{
			std::lock_guard<std::mutex> guard(m_);
			tasks_.push_back(task);
		}
		cv_.notify_one();
	} //addTask()
	~ThreadPool() {
		{
			std::lock_guard<std::mutex> guard(m_);
			active_ = false;
		}
		cv_.notify_all();
		for (auto& thread: threads_) {
			thread.join();
		}
	} //~ThreadPool()
private:
	void worker_() {
		task_t task = nullptr;
		while (true) {
			{
				std::unique_lock<std::mutex> lock(m_);
				if (!tasks_.empty()) {
					task = tasks_.front();
					tasks_.erase(tasks_.begin());
				} else if (!active_) {
					break;
				} else {
					cv_.wait(lock);
					continue;
				}
			}
			if (task) {
				task();
				task = nullptr;
			}
		}
	}
	bool active_;
	std::list<task_t> tasks_;
	std::mutex m_;
	std::vector<std::thread> threads_;
	std::condition_variable cv_;
};

#endif /* INCLUDE_THREAD_POOL_HPP_ */
