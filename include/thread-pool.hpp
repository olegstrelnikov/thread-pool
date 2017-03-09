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

class ThreadPool {
public:
	ThreadPool(std::size_t threadNumber, std::size_t usec = 1) : active_(true), usec_(usec) {
		threads_.reserve(threadNumber);
		for (; threadNumber; --threadNumber) {
			threads_.emplace_back(&ThreadPool::worker_, this);
		}
	}
	typedef void (*task_t)();
	void addTask(task_t task) {
		std::lock_guard<std::mutex> guard(m_);
		tasks_.push_back(task);
	}
	~ThreadPool() {
		while (true) {
			{
				std::lock_guard<std::mutex> guard(m_);
				if (tasks_.empty()) {
					break;
				}
			}
			std::this_thread::sleep_for(std::chrono::microseconds(usec_));
		}
		active_ = false;
		for (auto& thread: threads_) {
			thread.join();
		}
	}
private:
	void worker_() {
		task_t task = nullptr;
		while (active_) {
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
			} else {
				std::this_thread::sleep_for(std::chrono::microseconds(usec_));
			}
		}
	}
	bool active_;
	std::size_t usec_;
	std::list<task_t> tasks_;
	std::mutex m_;
	std::vector<std::thread> threads_;
};

#endif /* INCLUDE_THREAD_POOL_HPP_ */
