/*
 * main.cpp
 *
 *  Created on: Mar 9, 2017
 *      Author: ois
 */


#include <thread-pool.hpp>

#include <iostream>
#include <map>

const std::size_t FUNCTIONS = 16;
std::size_t calls[FUNCTIONS];
typedef decltype(std::this_thread::get_id()) thread_id_t;
thread_id_t thread_ids[FUNCTIONS];

template <std::size_t N> void f() {
	static_assert(N > 0, "");
	static_assert(N <= FUNCTIONS, "");
	++calls[N - 1];
	thread_ids[N - 1] = std::this_thread::get_id();
}

int main() {
	std::size_t fails = 0;
	for (std::size_t j = 0; j < 1000; ++j) {
		{
			ThreadPool threadPool(4);
			threadPool.addTask(f<1>);
			threadPool.addTask(f<2>);
			threadPool.addTask(f<3>);
			threadPool.addTask(f<4>);
			threadPool.addTask(f<5>);
			threadPool.addTask(f<6>);
			threadPool.addTask(f<7>);
			threadPool.addTask(f<8>);
			threadPool.addTask(f<9>);
			threadPool.addTask(f<10>);
			threadPool.addTask(f<11>);
			threadPool.addTask(f<12>);
			threadPool.addTask(f<13>);
			threadPool.addTask(f<14>);
			threadPool.addTask(f<15>);
			threadPool.addTask(f<16>);
		}
		for (std::size_t i = 0; i < FUNCTIONS; ++i) {
			if (calls[i] != 1 + j) {
				++fails;
			}
		}
	} //for j
	for (std::size_t i = 0; i < FUNCTIONS; ++i) {
		std::cout << calls[i] << " ";
	}
	std::cout << "\n";
	std::map<thread_id_t, std::size_t> byThread;
	for (auto id: thread_ids) {
		++byThread[id];
	}
	if (byThread.size() > 4) {
		++fails;
	}
	for (auto pair: byThread) {
		std::cout << pair.first << ": " << pair.second << "\n";
	}
	std::cout << "fails: " << fails << "\n";
	return 0;
}
